/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/11/01 11:05:17 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string	Server::checkJoin(Client &client, std::deque<std::string> data)
{
	if (data.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(data[0])));
	else if (data.size() > 3)
		return (client.send_error(ERR_TOOMANYPARAMS(data[0])));
	
	std::deque<std::string> list_channel = parsingMultiArgs(data[1]);
	std::deque<std::string> list_password;
	if (data.size() == 3)
		list_password = parsingMultiArgs(data[2]);
	size_t	i = 0;
	for (std::deque<std::string>::iterator it = list_channel.begin(); it != list_channel.end(); it++)
	{
		if (this->findChannel(*it) == NULL)
		{
			if ((*it)[0] != '#')
				return (client.send_error(ERR_NOSUCHCHANNEL(*it)));
			this->createChannel(client, *it);
		}
		else
		{
			Channel	*refChann = this->findChannel(*it);
			if (refChann->getModeI() == true)
			{
				if (refChann->findClientByNick(*it, refChann->getList()) == NULL)
				{
					client.send_error(ERR_INVITEONLYCHAN(*it));
					continue ;
				}
			}
			if (refChann->getModeK() == true)
			{
				if (i < list_password.size() && refChann->getPassword() != list_password[i])
				{
					client.send_error(ERR_BADCHANNELKEY(*it));
					i++;
					continue ;
				}
				i++;
			}
			this->joinChannel(client, *refChann);
		}
	}
	
	return ("");
}

std::string	Server::checkPass(Client &client, std::deque<std::string> password)
{
	if (password.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(password[0])));

	if (password.size() > 2)
		return (client.send_error(ERR_TOOMANYPARAMS(password[0])));

	if (password.size() == 2 && this->getPassword() == password[1] && client.getStatus() == 0)
	{
		client.setStatus(1);
		return ("");
	}
	else if (client.getStatus() == 1)
		return (client.send_error(ERR_ALREADYREGISTRED));

	return (client.send_error(ERR_PASSWDMISMATCH));
}

std::string	Server::checkUser(Client& client, std::deque<std::string> data)
{
	if (client.getRealName() != "")
		return (client.send_error(ERR_ALREADYREGISTRED));

	if (data.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(data[0])));
	
	if (data.size() < 5)
		return (client.send_error(ERR_NEEDMOREPARAMS(data[0])));
	else if (data.size() > 5)
		return (client.send_error(ERR_TOOMANYPARAMS(data[0])));

	if (client.getUsername().size() != 0)
			return (client.send_error(ERR_ALREADYREGISTRED));

	for (std::deque<std::string>::iterator it = data.begin(); it != data.end(); it++)
	{
		if ((*it).find('@') != std::string::npos || (*it).find('#') != std::string::npos)
			return (client.send_error(ERR_TOOMANYPARAMS(data[0])));
	}

	client.setUsername(data[1]);
	client.setRealName(data[4]);
	if (client.getNickname().size() != 0)
		client.setStatus(true);
	if (client.getUsername().size() != 0 && client.getNickname().size() != 0)
		return (client.send_error(AUTHENTIFICATED(client.getNickname())));
	return (client.send_error(SELECTUSER(client.getUsername())));
}

std::string	Server::checkNick(Client &client, std::deque<std::string> list_arg)
{
	if (list_arg.size() == 1 || list_arg[1] == "")
		return (client.send_error(ERR_NONICKNAMEGIVEN));

	if (list_arg[0].find('@') != std::string::npos || list_arg[0].find('#') != std::string::npos)
		return (client.send_error(ERR_TOOMANYPARAMS(list_arg[0])));

	if (this->findClientByNick(list_arg[1]) != NULL)
	{
		client.setDisconnected(true);
		return (client.send_error(ERR_NICKNAMEINUSE(list_arg[0])));
	}
	
	if (client.getNickname().size() == 0)
	{
		client.setNickname(list_arg[1]);
		return (client.send_error(SELECTNICKNAME(client.getNickname())));		
	}

	if (client.getStatus() == true)
		this->sendToAllClient(client, list_arg[1]);
	client.setNickname(list_arg[1]);
	if (client.getUsername().size() != 0 && client.getStatus() == false)
	{
		client.setStatus(true);
		return (client.send_error(AUTHENTIFICATED(client.getNickname())));
	}
	return (client.send_error(CHANGENICKNAME(client.getNickname()))); 
}

std::string	Server::checkPrivmsg(Client &client, std::deque<std::string> data)
{
	std::string	msg_to_send;

	std::deque<std::string>	receiver = data;

	receiver.pop_front();
	if (receiver.size() == 0)
		return (client.send_error(ERR_NORECIPIENT(data[0])));
	else if (receiver.size() == 1)
		return (client.send_error(ERR_NOTEXTTOSEND));
	else if (receiver.size() > 2)
		return (client.send_error(ERR_TOOMANYPARAMS(data[0])));

	receiver = parsingMultiArgs(data[1]);
	msg_to_send = data[data.size() - 1];

	for (std::deque<std::string>::iterator it = receiver.begin(); it != receiver.end(); it++)
	{
		if ((*it)[0] != '#')
			this->sendToClient(client, *it, msg_to_send);
		else
			this->sendToChannel(client, *it, msg_to_send);
	}
	return ("");
}

// std::string	Server::checkMode(Client &client, std::deque<std::string> data)
// {
	
// }
