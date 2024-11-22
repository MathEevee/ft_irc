/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:32:23 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/20 16:50:48 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

std::string	Channel::addOp(Client &client, std::deque<std::string> data, size_t &i)
{
	if (i >= data.size())
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));

	std::deque<std::string>	list_user = parsingMultiArgs(data[i]);
	for (std::deque<std::string>::iterator it = list_user.begin(); it != list_user.end(); it++)
	{
		if (this->findClientByNick(*it, this->getAllClient()))
		{
			if (this->findClientByNick(*it, this->getClientOp()) == NULL)
			{
				this->sendAllClient(client, MSGOP(client.getNickname(), client.getUsername(), client.getIp(), data[1], "+o", *it));
				this->getClientOp().push_back(findClientByNick(*it, this->getAllClient()));
			}
			else
				client.send_error(ERR_CHANALREADYOP(client.getNickname(), *it));
		}
		else
			client.send_error(ERR_NOSUCHNICK(client.getNickname(), *it));
	}
	i++;
	return ("");
}

std::string	Channel::removeOp(Client &client, std::deque<std::string> data, size_t &i)
{
	if (i >= data.size())
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));

	std::deque<std::string>	list_user = parsingMultiArgs(data[i]);
	for (std::deque<std::string>::iterator it = list_user.begin(); it != list_user.end(); it++)
	{
		if (this->findClientByNick(*it, this->getAllClient()))
		{
			if (this->findClientByNick(*it, this->getClientOp()) != NULL)
			{
				this->sendAllClient(client, MSGOP(client.getNickname(), client.getUsername(), client.getIp(), data[1], "-o", *it));
				this->deleteClient(*findClientByNick(*it, this->getAllClient()), this->getClientOp());
			}
			else
				client.send_error(ERR_NOSUCHNICK(client.getNickname(), *it));
		}
		else
			client.send_error(ERR_NOSUCHNICK(client.getNickname(), *it));
	}
	i++;
	return ("");
}

std::string Channel::execModeI(Client &client, char token)
{
	if (token == '+')
	{
		if (this->getModeI() == true)
			return ("");
		this->setModeI(true);
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "+i")));
	}
	else if (this->getModeI() == true && token == '-')
	{
		this->setModeI(false);
		this->getList().clear();
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "-i")));
	}
	return ("");
}

std::string Channel::execModeT(Client &client, char token)
{
	if (token == '+')
	{
		this->setModeT(true);
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "+t")));
	}
	else if (this->getModeT() == true && token == '-')
	{
		this->setModeT(false);
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "-t")));
	}
	return ("");
}

std::string Channel::execModeK(Client &client, std::deque<std::string> data, size_t &i, char token)
{
	if (this->getModeK() == false && token == '+')
	{
		if (i >= data.size() || data[i].size() == 0)
			return ("");
		this->setModeK(true);
		this->setPassword(data[i]);
		i++;
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "+k")));
	}
	else if (this->getModeK() == true && token == '-')
	{
		this->setModeK(false);
		this->setTopic("");
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "-k")));
	}
	else if (this->getModeK() == true && token == '+')
		return (sendAllClient(client, ERR_KEYSET(client.getNickname(), this->getName())));
	return ("");
}

std::string	Channel::execModeL(Client &client, std::deque<std::string> data, size_t &i, char token)
{
	if (token == '+')
	{
		if (i >= data.size())
			return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
		if (std::atoll(data[i].c_str()) <= 0)
			return ("");
		this->setModeL(true);
		this->setNbrClient(std::atoll(data[i].c_str()));
		std::string	nbClient = data[i];
		i++;
		return (sendAllClient(client, MSGOP(client.getNickname(), client.getUsername(), client.getIp(), data[1], "+l", nbClient)));
	}
	else if (this->getModeL() == true && token == '-')
	{
		this->setModeL(false);
		this->setNbrClient(0);
		return (sendAllClient(client, CHANNELMODEJOIN(this->getName(), "-l")));
	}
	return ("");
}
//TODO remove
void	Channel::print(std::string sender, std::deque<Client*> &list, std::string msg)
{
	std::cout << sender << " : " << msg << std::endl;
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		std::cout << "nick name = " << (*it)->getNickname() << std::endl;	
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

Client*	Channel::findClientByNick(std::string sender, std::deque<Client*> &list)
{
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		if (*it == NULL)
		{
			std::cout << "NULL" << std::endl;
			continue;
		}
		if ((*it)->getNickname() == sender)
			return ((*it));
	}
	return (NULL);
}

std::string	Channel::sendAllClientMsg(Client &sender, std::string msg)
{
	if (this->findClientByNick(sender.getNickname(), this->_list_client) == NULL)
		return (ERR_CANNOTSENDTOCHAN(this->getName()));

	for (std::deque<Client*>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		if (sender.getNickname() != (*it)->getNickname())
			send((*it)->getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	return ("");
}

std::string	Channel::sendAllClient(Client &sender, std::string msg)
{
	if (this->findClientByNick(sender.getNickname(), this->_list_client) == NULL)
		return (ERR_CANNOTSENDTOCHAN(this->getName()));

	for (std::deque<Client*>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		send((*it)->getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	return ("");
}

void	Channel::deleteClient(Client &client, std::deque<Client*> &list)
{
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it)->getNickname() == client.getNickname())
		{
			list.erase(it);
			return ;
		}
	}
}

void	Channel::removeClient(Client &client)
{
	if (this->getModeI() == true || this->findClientByNick(client.getNickname(), this->getList()) != NULL)
		this->deleteClient(client, this->getList());

	if (this->findClientByNick(client.getNickname(), this->getClientOp()) != NULL)
	{
		//TODO send msg to remove op
		deleteClient(client, this->getClientOp());
	}
	if (this->findClientByNick(client.getNickname(), this->getAllClient()) != NULL)
	{
		this->sendAllClient(client, CHANNELLEAVE(client.getNickname(), client.getUsername(), client.getIp(), this->getName()));
		deleteClient(client, this->getAllClient());
	}
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
	_mode_o.push_back(&new_client);
	addClient(new_client);
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

std::deque<Client*>&	Channel::getList(void)
{
	return (this->_invite_list);
}

std::deque<Client*>&	Channel::getClientOp(void)
{
	return (this->_mode_o);
}

std::deque<Client*>&	Channel::getAllClient(void)
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
	for (std::deque<Client*>::iterator it = this->getAllClient().begin(); it != this->getAllClient().end(); it++)
	{
		if (it == this->getAllClient().end())
			break;
		if (this->findClientByNick((*it)->getNickname(), this->getClientOp()))
			user += std::string("@");
		user += std::string((*it)->getNickname());
		if (it != this->getAllClient().end())
			user += std::string(" ");
	}
	return (user);
}


void	Channel::sendMsgJoin(Client &client)
{
	std::string	msg = CHANNELMODEJOIN(this->getName(), this->getAllMode());
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	msg = CHANNELLIST(client.getNickname(), this->getName(),this->getAllUser());
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	if (this->getModeT() == true)
	{
		if (this->getTopic().size() == 0)
			msg = RPL_NOTOPIC(this->getName());
		else
			msg = RPL_TOPIC(client.getNickname(), this->getName(), this->getTopic());
		send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	msg = CHANNELEND(client.getNickname(), this->getName());
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
}

void	Channel::addClient(Client &new_client)
{
	this->_list_client.push_back(&new_client);
	this->sendAllClient(new_client, MSGJOIN(new_client.getNickname(), new_client.getUsername(), new_client.getIp(), this->getName()));
	this->sendMsgJoin(new_client);
}
