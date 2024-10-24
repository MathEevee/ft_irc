/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/24 17:15:20 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string	Server::checkPass(Client &client, std::deque<std::string> password)
{
	if (password.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(password[0])));

	if (password.size() > 2)
		return (client.send_error(ERR_TOOMANYPARAMS(password[0])));

	if (password.size() == 1 && this->getPassword() == password[1] && client.getStatus() == 0)
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
	int i = 0;

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
		return (AUTHENTIFICATED(client.getNickname()));
	return ("");
}

std::string	Server::checkNick(Client &client, std::deque<std::string> list_arg)
{
	if (list_arg.size() == 1 || list_arg[1] == "")
		return (client.send_error(ERR_NONICKNAMEGIVEN));

	if (list_arg[0].find('@') != std::string::npos || list_arg[0].find('#') != std::string::npos)
		return (client.send_error(ERR_TOOMANYPARAMS(list_arg[0])));

	if (this->findClientByNick(list_arg[1]) != NULL)
			return (client.send_error(ERR_NICKNAMEINUSE(list_arg[0])));
	
	if (client.getUsername().size() != 0 && client.getNickname().size() == 0)
	{
		client.setNickname(list_arg[1]);
		return (client.send_error(AUTHENTIFICATED(client.getNickname())));		
	}

	client.setNickname(list_arg[1]);
	return (""); 
}



std::string	Server::checkPrivmsg(Client &client, std::deque<std::string> data)
{
	if (data.size() < 3)
		return (client.send_error());
	std::deque<std::string>	receiver = data;
	receiver.pop_back();
	receiver.pop_front();
	for ()
}
