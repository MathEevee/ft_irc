/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/24 15:00:21 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Error.hpp"

std::string	Server::checkPass(Client &client, std::deque<std::string> password)
{
	if (password.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), password[0])));
	password.pop_front();
	std::string	check_password = password[0];
	if (check_password[0] == ':')
		check_password = check_password.substr(1);
	if (password.size() == 1 && this->getPassword() == check_password && client.getStatus() == 0)
	{
		client.setStatus(1);
		return ("");
	}
	else if (client.getStatus() == 1)
		return (client.send_error(ERR_ALREADYREGISTRED(client.getNickname())));
	return (client.send_error(ERR_PASSWDMISMATCH(client.getNickname())));
}

std::string	Server::checkUser(Client& client, std::deque<std::string> data)
{
	int i = 0;

	if (client.getRealName() != "")
		return (client.send_error(ERR_ALREADYREGISTRED(client.getNickname())));

	if (data.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	
	if (data.size() < 5)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 5)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

	if (client.getUsername().size() != 0)
			return (ERR_ALREADYREGISTRED(client.getNickname()));

	for (std::deque<std::string>::iterator it = data.begin(); it != data.end(); it++)
	{
		if ((*it).find('@') != std::string::npos || (*it).find('#') != std::string::npos)
			return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
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
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), list_arg[0])));

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



// std::string	Server::checkPrivmsg(Client &client, std::deque<std::string> data)
// {
// 	std::string recipient = data.substr(0, data.find(' '));
	
// 	if (recipient[0] == '#')
// 	{
// 		std::cout << "send in channel : " << recipient << " : " << data.substr(data.find(' ') + 1) << std::endl; 
// 		//checkChannel & send message or error, channel doesn't exist
// 	}
// 	else
// 	{
// 		Client *interlocutor = this->findClientByNick(recipient);
// 		if (interlocutor == NULL)
// 		{
// 			std::cout << "976 can't send message to user :" << recipient << std::endl;
// 			//send error interlocutor doesn't exist
// 		}
// 		else
// 		{
// 			client.send_private_message(*interlocutor, data.substr(data.find(' ') + 1));
// 		}
			
// 	}
// 	return (false);
// }
