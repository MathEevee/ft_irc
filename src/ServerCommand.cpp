/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:58:11 by mbriand           #+#    #+#             */
/*   Updated: 2024/11/22 14:08:01 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kick(Client &client, Channel &channel, std::string target, std::string msg)
{
	Client *kicked = this->findClientByNick(target);
	
	if (channel.findClientByNick(target, channel.getClientOp()) != NULL)
	{
		channel.sendAllClient(client, MSGOP(client.getNickname(), client.getUsername(), client.getIp(), channel.getName(), "-o", target));
		channel.deleteClient(*findClientByNick(target), channel.getClientOp());
	}
	channel.deleteClient(*kicked, channel.getAllClient());
	kicked->send_error(KICK(client.getNickname(), client.getUsername(), client.getIp(), target, channel.getName(), msg));
	channel.sendAllClient(client, KICK(client.getNickname(), client.getUsername(), client.getIp(), target, channel.getName(), msg));
	this->checkDeleteChannel();

}


std::string	Server::checkKick(Client &client, std::deque<std::string> data)
{
	if (data.size() < 3)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 4)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
	
	std::string	msg = " ";
	if (data.size() == 4)
	{
		if (data[3][0] != ':')
			msg = " :" + data[3];
		else
			msg += data[3];
	}
	else
	{
		if (data[2][0] != ':')
			msg = " :" + data[2];
		else
			msg += data[2];
	}
	
	std::deque<std::string>	channel = parsingMultiArgs(data[1]);
	std::deque<std::string>	clients = parsingMultiArgs(data[2]);
	
	for (std::deque<std::string>::iterator it = channel.begin(); it != channel.end(); it++)
	{
		Channel *channel = this->findChannel(*it);
		if (channel == NULL)
		{
			client.send_error(ERR_NOSUCHCHANNEL(*it));
			continue;
		}
		if (channel->findClientByNick(client.getNickname(), channel->getAllClient()) == NULL)
		{
			client.send_error(ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
			continue;
		}
		if (channel->findClientByNick(client.getNickname(), channel->getClientOp()) == NULL)
		{
			client.send_error(ERR_CHANNOTOPSNEEDED(client.getNickname(), channel->getName()));
			continue;
		}
		for (std::deque<std::string>::iterator target = clients.begin(); target != clients.end(); target++)
		{
			if (channel->findClientByNick(*target, channel->getAllClient()) == NULL)
				client.send_error(ERR_NOTONCHANNEL(client.getNickname(), *target));
			else
				kick(client, *channel, *target, msg);
		}
	}
	return ("");
}


std::string	Server::checkInvite(Client &client, std::deque<std::string> data)
{
	if (data.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 3)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
	
	if (this->findClientByNick(data[2]) == NULL)
		return (client.send_error(ERR_NOSUCHNICK(client.getNickname(), data[2])));
	
	Channel *channel = this->findChannel(data[1]);

	if (channel == NULL)
		return (client.send_error(ERR_NOSUCHCHANNEL(data[1])));

	if (channel->findClientByNick(client.getNickname(), this->findChannel(data[1])->getAllClient()) == NULL)
		return (client.send_error(ERR_NOTONCHANNEL(client.getNickname(), data[1])));

	if (channel->findClientByNick(client.getNickname(), this->findChannel(data[1])->getClientOp()) == NULL)
		return (client.send_error(ERR_CHANNOTOPSNEEDED(client.getNickname(), data[1])));

	if (channel->findClientByNick(data[2], this->findChannel(data[1])->getAllClient()) != NULL)
		return (client.send_error(ERR_USERONCHANNEL(client.getNickname(), data[1])));

	Client *receiver = this->findClientByNick(data[2]);

	channel->getList().push_back(receiver);
	channel->print(client.getNickname(), channel->getList(), "list invite");
	receiver->send_error(INVITE(client.getNickname(), client.getUsername(), client.getIp(), receiver->getNickname(), data[1]));
	return (client.send_error(INVITESENDER(client.getNickname(), data[2], data[1])));
}

std::string	Server::checkJoin(Client &client, std::deque<std::string> data)
{
	if (data.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 3)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
	
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
				if (refChann->findClientByNick(client.getNickname(), refChann->getList()) == NULL)
				{
					refChann->print(client.getNickname(), refChann->getList(), "check sortie");
					client.send_error(ERR_INVITEONLYCHAN(*it));
					continue ;
				}
			}
			if (refChann->getModeK() == true)
			{
				if (i >= list_password.size() || refChann->getPassword() != list_password[i])
				{
					client.send_error(ERR_BADCHANNELKEY(*it));
					i++;
					continue ;
				}
				i++;
			}
			if (refChann->findClientByNick(*it, refChann->getList()) != NULL)
				refChann->deleteClient(client, refChann->getList());
			this->joinChannel(client, *refChann);
		}
	}
	
	return ("");
}

std::string	Server::checkPass(Client &client, std::deque<std::string> password)
{
	if (password.size() == 1)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), password[0])));

	if (password.size() > 2)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), password[0])));

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

	if (data.size() < 5)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 5)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

	if (client.getUsername().size() != 0)
			return (client.send_error(ERR_ALREADYREGISTRED));

	for (std::deque<std::string>::iterator it = data.begin(); it != data.end(); it++)
	{
		if ((*it).find('@') != std::string::npos || (*it).find('#') != std::string::npos)
			return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
	}

	client.setUsername(data[1]);
	client.setRealName(data[4]);
	if (client.getNickname().size() != 0)
		client.setStatus(true);
	if (client.getUsername().size() != 0 && client.getNickname().size() != 0)
	{
		client.send_error(SELECTUSER(client.getUsername()));
		return (client.send_error(AUTHENTIFICATED(client.getNickname())));
	}
	return (client.send_error(SELECTUSER(client.getUsername())));
}

std::string	Server::checkNick(Client &client, std::deque<std::string> list_arg)
{
	if (list_arg.size() == 1 || list_arg[1] == "")
		return (client.send_error(ERR_NONICKNAMEGIVEN));

	if (list_arg[0].find('@') != std::string::npos || list_arg[0].find('#') != std::string::npos)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), list_arg[0])));

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

	std::cout << "New nick : " << list_arg[1] << std::endl;
	client.setNickname(list_arg[1]);
	if (client.getUsername().size() != 0 && client.getStatus() == false)
	{
		client.setStatus(true);
		return (client.send_error(AUTHENTIFICATED(client.getNickname())));
	}
	return (client.send_error(CHANGENICKNAME(client.getNickname()))); //TODO Verify
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
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

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

std::string	Server::checkTopic(Client &client, std::deque<std::string> data)
{
	if (data.size() < 2)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));

	if (this->findChannel(data[1]) == NULL)
		return (client.send_error(ERR_NOSUCHCHANNEL(data[1])));

	if (this->findChannel(data[1])->findClientByNick(client.getNickname(), this->findChannel(data[1])->getAllClient()) == NULL)
		return (client.send_error(ERR_NOTONCHANNEL(client.getNickname(),data[1])));
	
	if (data.size() == 2)
	{
		if (this->findChannel(data[1])->getTopic().size() == 0)
			return (client.send_error(RPL_NOTOPIC(data[1])));
		else
			return (client.send_error(RPL_TOPIC(client.getNickname(), data[1], this->findChannel(data[1])->getTopic())));
	}

	if (data.size() > 3)
		return (client.send_error(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

	if (data[2][0] != ':')
	{
		std::string	topic = ":" + data[2];
		this->findChannel(data[1])->setTopic(topic);
	}
	else
		this->findChannel(data[1])->setTopic(data[2]);
	return (this->findChannel(data[1])->sendAllClient(client, CHANGEDTOPIC(client.getNickname(), client.getUsername(), client.getIp(), data[1], this->findChannel(data[1])->getTopic())));
}

std::string	Server::checkMode(Client &client, std::deque<std::string> data)
{
	char	token = '+';


	if (data.size() < 2)
		return (client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));

	if (this->findChannel(data[1]) == NULL)
		return (client.send_error(ERR_NOSUCHCHANNEL(data[1])));

	if (data.size() == 2)
		return (client.send_error(CHANNELMODE(client.getNickname(), this->findChannel(data[1])->getName(), this->findChannel(data[1])->getAllMode())));
	
	if (this->findChannel(data[1])->findClientByNick(client.getNickname(), this->findChannel(data[1])->getClientOp()) == NULL)
		return (client.send_error(ERR_CHANNOTOPSNEEDED(client.getNickname(), data[1])));

	size_t	i = 3;
	for (std::string::iterator it = data[2].begin(); it != data[2].end(); it++)
	{
		if (it == data[2].end())
			//TODO return ?
		if (this->findChannel(data[1])->findClientByNick(client.getNickname(), this->findChannel(data[1])->getClientOp()) == NULL)
			return (client.send_error(ERR_CHANNOTOPSNEEDED(client.getNickname(), data[1])));
		if (*it == '-' || *it == '+')
			token = *it;
		else
			this->execMode(client, data, i, token, *it, *this->findChannel(data[1]));
	}
	return ("");
}
//TODO i l or mode no params
void	Server::execMode(Client &client, std::deque<std::string> data, size_t &i, char token, char mode, Channel &channel)
{
	if (mode == 'i')
		channel.execModeI(client, token);
	else if (mode == 't')
		channel.execModeT(client, token);
	else if (data.size() < i && token == '+')
		client.send_error(ERR_NEEDMOREPARAMS(client.getNickname(), data[0]));
	else if (mode == 'k')
		channel.execModeK(client, data, i, token);
	else if (mode == 'o')
	{
		if (token == '-')
			channel.removeOp(client, data, i);
		else
			channel.addOp(client, data, i);
	}
	else if (mode == 'l')
		channel.execModeL(client, data, i, token);
	else
		client.send_error(ERR_UNKNOWNMODE(client.getNickname(), mode));
}
