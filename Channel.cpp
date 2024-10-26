/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:32:23 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/26 18:19:30 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Client*	Channel::findClientByNick(std::string sender, std::deque<Client> list)
{
	for (std::deque<Client>::iterator it = list.begin(); it != list.end(); it++)
	{
		if (it->getNickname() == sender)
			return (&(*it));
	}
	return (NULL);
}



std::string	Channel::sendAllClient(Client &sender, std::string msg)
{
	if (this->findClientByNick(sender.getNickname(), this->_list_client) == NULL)
		return (ERR_CANNOTSENDTOCHAN(this->getName()));
	for (std::deque<Client>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		send((*it).getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	return ("");
}

Channel::Channel(std::string name, Client &new_client)
{
	setName(name);
	addClient(new_client);
	_mode_i = false;
	_mode_t = false;
	_mode_k = false;
	_mode_l = false;
	_mode_o.push_back(new_client);
	_invite_list.push_back(new_client);
}

Channel::~Channel()
{}
		
std::string	Channel::getName(void)
{
	return (this->_name);
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}
		
void	Channel::addClient(Client &new_client)
{
	this->_list_client.push_back(new_client);	
}