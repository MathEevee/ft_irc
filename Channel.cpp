/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:32:23 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/01 12:05:06 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Client*	Channel::findClientByNick(std::string sender, std::deque<Client> &list)
{
	for (std::deque<Client>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it).getNickname() == sender)
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
		if (it->getNickname() == sender.getNickname())
			send((*it).getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	return ("");
}

void	Channel::deleteClient(Client &client, std::deque<Client> &list)
{
	for (std::deque<Client>::iterator it = list.begin(); it != list.end(); it++)
	{
		if (it->getNickname() == client.getNickname())
		{
			list.erase(it);
			return ;
		}
	}
}

void	Channel::removeClient(Client &client)
{
	if (this->getModeI() == true)
		this->deleteClient(client, this->getList());

	deleteClient(client, this->getClientOp());
	deleteClient(client, this->getAllClient());
	this->sendAllClient(client, CHANNELLEAVE(client.getNickname(), client.getUsername(), client.getIp(), this->getName()));
}

Channel::Channel()
{
	setModeI(false);
	setModeK(false);
	setModeL(false);
	setModeT(false);
	setTopic("");
	setPassword("");
	setNbrClient(0);
}

Channel::Channel(std::string name, Client &new_client)
{
	setName(name);
	
	setModeI(false);
	setModeK(false);
	setModeL(false);
	setModeT(false);
	setTopic("");
	addClient(new_client);
	_mode_o.push_back(new_client);
	_invite_list.push_back(new_client);
	setPassword("");
	setNbrClient(0);
}

Channel::~Channel()
{}
		
std::string	Channel::getName(void)
{
	return (this->_name);
}

void	Channel::setPassword(std::string password)
{
	this->_password = password;
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}

void	Channel::setNbrClient(size_t nbr)
{
	this->_nbr_client = nbr;
}

void	Channel::setModeI(bool i)
{
	this->_mode_i = i;
}

void	Channel::setModeK(bool k)
{
	this->_mode_k = k;
}

void	Channel::setModeT(bool t)
{
	this->_mode_t = t;
}

void	Channel::setModeL(bool l)
{
	this->_mode_l = l;
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

bool	Channel::getModeK(void)
{
	return (this->_mode_k);
}

bool	Channel::getModeI(void)
{
	return (this->_mode_i);
}

bool	Channel::getModeL(void)
{
	return (this->_mode_l);
}

bool	Channel::getModeT(void)
{
	return (this->_mode_t);
}

std::deque<Client>&	Channel::getList(void)
{
	return (this->_invite_list);
}

std::deque<Client>&	Channel::getClientOp(void)
{
	return (this->_mode_o);
}

std::deque<Client>&	Channel::getAllClient(void)
{
	return (this->_list_client);
}

size_t	Channel::getNbrClient(void)
{
	return (this->_nbr_client);
}


std::string	Channel::getPassword(void)
{
	return (this->_password);
}

std::string	Channel::getTopic(void)
{
	return (this->_topic);
}

std::string	Channel::getAllMode(void)
{
	std::string allMode = "+";
	if (this->getModeK() == true)
		allMode += std::string("k");
	if (this->getModeI() == true)
		allMode += std::string("i");
	if (this->getModeL() == true)
		allMode += std::string("l");
	if (this->getModeT() == true)
		allMode += std::string("t");
	return (allMode);
}

std::string	Channel::getAllUser(void)
{
	std::string user;	
	for (std::deque<Client>::iterator it = this->getAllClient().begin(); it != this->getAllClient().end(); it++)
	{
		Client *test = this->findClientByNick(it->getNickname(), this->getClientOp());
		std::cout << test->getNickname() << std::endl;
		if (this->findClientByNick(it->getNickname(), this->getClientOp()))
		{
			std::cout << "ici" << std::endl;
			user += "@";
		}
		user += it->getNickname();
		if (it != this->getAllClient().end())
			user += " ";
	}
	return (user);
}


void	Channel::sendMsgJoin(Client &client)
{
	std::string	msg = CHANNELMODE(this->getName(), this->getAllMode());
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	if (this->getModeT() == true)
	{
		msg = CHANELLTOPIC(client.getNickname(), this->getName(), this->getTopic());
		send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	msg = CHANNELLIST(client.getNickname(), this->getName(),this->getAllUser());
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	msg = CHANNELEND(client.getNickname(), this->getName());
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
}

void	Channel::addClient(Client &new_client)
{
	this->_list_client.push_back(new_client);
	this->sendAllClient(new_client, MSGJOIN(new_client.getNickname(), new_client.getUsername(), new_client.getIp(), this->getName()));
	this->sendMsgJoin(new_client);
}