/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/10/22 17:16:44 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Check is the provide password is the good password
bool	Server::checkPass(Client &client, std::string password)
{
	std::cout << "|" << password << "|"  << std::endl;
	if (this->getPassword() == password && client.getStatus() == 0)
	{
		std::cout << "Client authentificated." << std::endl;
		std::string client_msg = ":" + _name + ": Your are authentificated." + "\r\n";
		send(client.getSocketFd(), client_msg.c_str(), client_msg.size(), 0);
		client.setStatus(1);
		return (true);
	}
	else if (client.getStatus() == 1)
	{
		client.send_error(462, ":You may not reregister");
		return (true);
	}
	client.send_error(464, "Failed password attempt.");
	return (false);
}

// modify function using client informations
bool	Server::checkUser(Client& client, std::string data)
{
	std::string username;
	std::string real_name;
	std::string tmp = data;
	int i = 0;

	if (client.getStatus() == 0)
	{
		//add error_send
		std::cout << "Client not connected, use command 'PASS'" << std::endl;
		return (false);
	}
	
	// checkData for #, @
	std::cout << data << std::endl;
	for (std::string::iterator it = data.begin(); it != data.end(); it++)
	{
		if (isalnum(*it) == 0 && *it != ' ' && *it != '-' && *it != '_' && *it != '`' && *it != '^' && *it != '{' && *it != '}')
		{
			client.send_error(421, "USER :Unknown command");
			return (false);
		}
	}
	
	if (!client.getUsername().size())
	{
		if (data.find(' ') == std::string::npos)
			return (false);

		while (i != 4)
		{
			if (i == 0)	
				username = data.substr(0, data.find(' '));
			if (i == 3)
				real_name = data.substr(data.find_last_of(' ') + 1);
			tmp = tmp.substr(tmp.find(' ') + 1);
			i++;
		}

		if (tmp != real_name)
		{
			//error to many args
			client.send_error(461, "USER :Not enough parameters");
			return (false);
		}
		client.setUsername(username);
		client.setRealName(real_name);
		//client set Username
		return (true);
	}
	return (false);
}


// PRIV <nickname> <message>
bool	Server::checkPrivmsg(Client &client, std::string data)
{
	std::string recipient = data.substr(0, data.find(' '));
	
	if (recipient[0] == '#')
	{
		std::cout << "send in channel : " << recipient << " : " << data.substr(data.find(' ') + 1) << std::endl; 
		//checkChannel & send message or error, channel doesn't exist
	}
	else
	{
		Client *interlocutor = this->findClientByNick(recipient);
		if (interlocutor == NULL)
		{
			std::cout << "976 can't send message to user :" << recipient << std::endl;
			//send error interlocutor doesn't exist
		}
		else
		{
			client.send_private_message(*interlocutor, data.substr(data.find(' ') + 1));
		}
			
	}
	return (false);
}
