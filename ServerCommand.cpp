/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/23 17:56:58 by matde-ol         ###   ########.fr       */
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
	std::deque<std::string> args;
	std::string				params;
	int i = 0;

	if (client.getRealName() != "")
		return (client.send_error(ERR_ALREADYREGISTRED(client.getNickname())));
	if (data.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));

	params = data[1];
	if (data[1][0] == ':')
		params = data[1].substr(1);
	args = splitCommand(params);

	if (args.size() < 4)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (args.size() > 4)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

	if (client.getUsername().size() != 0)
			return (ERR_ALREADYREGISTRED(client.getNickname()));

	for (std::deque<std::string>::iterator it = args.begin(); it != args.end(); it++)
	{
		if ((*it).find('@') != std::string::npos || (*it).find('#') != std::string::npos)
			return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
	}

	client.setUsername(args[0]);
	client.setRealName(args[3]);
	if (client.getNickname().size() != 0)
	{
		client.connexionFull();
		return ("");
	}
	// 	//client set Username
	// return (false);
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
