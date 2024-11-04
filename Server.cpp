/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:26:06 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/04 16:20:54 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::deque<std::string>	parsingMultiArgs(std::string data)
{
	std::deque<std::string> tab;

	size_t found = data.find(',');
	std::string	receiver = data.substr(0, found);
	tab.push_back(receiver);
	while (found != std::string::npos)
	{
		if (receiver.size() != 0)
			tab.push_back(receiver);
		receiver = data.substr(found + 1, data.find(',', found) + 1);
		found = data.find(',', found + receiver.size());
	}
	if (receiver != tab[tab.size() - 1])
		tab.push_back(receiver);
	return (tab);
}

void	Server::createChannel(Client &client, std::string name)
{
	Channel new_channel(name, client);

	this->_channel_list.push_back(new_channel);
	new_channel.print(client.getNickname(), new_channel.getAllClient(), "all clients");
	new_channel.print(client.getNickname(), new_channel.getClientOp(), "op client");
	new_channel.print(client.getNickname(), new_channel.getList(), "invit list");

}

void	Server::joinChannel(Client &client, Channel &channel) const
{
	if (channel.getModeL() == true)
	{
		if (channel.getList().size() < channel.getNbrClient())
			channel.addClient(client);
		else
			client.send_error(ERR_CHANNELISFULL(channel.getName()));
	}
	else
		channel.addClient(client);
}


void	Server::sendToAllClient(Client &client, std::string new_nickname)
{
	std::string	toSend = CHANGENICKNAMEFORALL(client.getNickname(), new_nickname);
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		if ((*it)->getNickname() != client.getNickname())
			send((*it)->getSocketFd(), toSend.c_str(), toSend.size(), 0);
	}
}

Channel*	Server::findChannel(std::string channel)
{
	for (std::vector<Channel>::iterator it = this->_channel_list.begin(); it != this->_channel_list.end(); it++)
	{
		if (it->getName() == channel)
			return (&(*it));
	}
	return (NULL);
}

std::string Server::sendToChannel(Client &sender, std::string channel, std::string msgToSend)
{
	std::string	msg = MSGSEND(sender.getNickname(), sender.getUsername(), sender.getIp(), channel, msgToSend);
	
	if (this->findChannel(channel) == NULL)
		return (sender.send_error(ERR_NOSUCHNICK(sender.getNickname(), channel)));

	this->findChannel(channel)->sendAllClientMsg(sender, msg);
	return ("");
}

std::string Server::sendToClient(Client &sender, std::string receiver, std::string msgToSend)
{
	std::string	msg = MSGSEND(sender.getNickname(), sender.getUsername(), sender.getIp(), receiver, msgToSend);
	
	if (this->findClientByNick(receiver) == NULL)
		return (sender.send_error(ERR_NOSUCHNICK(sender.getNickname(), receiver)));
	send(this->findClientByNick(receiver)->getSocketFd(), msg.c_str(), msg.size(), 0);
	return ("");
}


std::deque<std::string>	splitCommand(std::string input)
{
	std::deque<std::string>		tab;
	std::string					arg;
	bool						end_command = false;


	if (input[input.size() - 1] == '\r')
		input = input.substr(0, input.size() - 1);
	for (std::string::iterator it = input.begin(); it != input.end(); it++)
	{
		if (*it == ' ' && end_command == false)
		{
			if (arg.size() != 0)
			{
				tab.push_back(arg);
				arg = "";
			}
		}
		else if (*it == ':')
			end_command = true;

		if (*it != ' ' || end_command == true)
			arg += *it;
	}
	if (arg.size() != 0)
		tab.push_back(arg);
	return (tab);
}

void	Server::runtime()
{
	struct 	pollfd fds[NB_MAX_CLIENTS + 1];

	while (1)
	{
		bool	new_client = false;
		initialize_poll_fds(fds);

		int	nb_fd = poll(fds, _client_list.size() + 1, -1);

		if (nb_fd == -1 && errno == EINTR)
			return;

		if ((fds[0].revents & POLLIN) != 0)
		{
			new_client = add_client();
		}

		if (new_client == true || this->_client_list.size() != 0)
			read_all_clients(fds, new_client);
	}
}

void	Server::initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1])
{
	fds[0].fd = this->getServerSocket();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int	i = 1;
	for (std::vector<Client*>::iterator it = _client_list.begin(); it != _client_list.end(); it++)
	{
		fds[i].fd = (*it)->getSocketFd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
		i++;
	}
}

Client*	Server::findClientByNick(std::string recipient)
{
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		if ((*it)->getNickname() == recipient)
			return ((*it));
	}
	return (NULL);
}

void	Server::leaveAllChannel(Client &client)
{
	std::vector<Channel> &channels = this->getListChannel();
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end();)
	{
		it->removeClient(client);
		if (it->getAllClient().size() == 0)
			it = channels.erase(it);
		else
			it++;
	}
}


bool	Server::add_client()
{
	sockaddr_in		addr;
	unsigned int	len = 0;

	std::memset(&addr, 0, sizeof(sockaddr_in));
	int clientSocket = accept(this->getServerSocket(), (struct sockaddr*)&addr, &len);
	if (clientSocket != -1)
	{
		if (this->_client_list.size() >= NB_MAX_CLIENTS)
		{
			close(clientSocket);
			std::cout << "Too many clients.\r" << std::endl;
		}
		else
		{
			char	ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
			Client	*new_client = new Client(clientSocket, ip);
			this->_client_list.push_back(new_client);
			std::cout << "New client connected.\r" << std::endl;
			return (true);
		}
	}
	return (false);
}

void	Server::sendToAll(Client &client)
{
	for (std::vector<Client*>::iterator it = this->getListClient().begin(); it != this->getListClient().end(); it++)
	{
		std::string	msgLeave = USERDISCONNECTED(client.getNickname(), client.getUsername(), client.getIp(), (*it)->getNickname());
		if ((*it)->getNickname() != client.getNickname())
			send((*it)->getSocketFd(), msgLeave.c_str(), msgLeave.size(), 0);
	}
}

void	Server::read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
{
	int		i = 1;
	ssize_t size;
	char	buffer[1024];

	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end() - new_client;)
	{
		std::string	message = (*it)->getMessage();

		if ((fds[i].revents & POLLIN) != 0)
		{
			do
			{
				size = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (size == 0)
					(*it)->setDisconnected(true);
				buffer[size] = 0;
				message = message + buffer;
				(*it)->setMessage(message);
			} while (size == 1024);

			this->process_commands(**it);
		}
		i++;

		if ((*it)->getDisconnected() == false)
			it++;
		else
		{
			this->leaveAllChannel(**it);
			this->sendToAll(**it);
			delete (*it);
			it = this->_client_list.erase(it);
		}
	}
}

bool	Server::process_commands(Client &client)
{
	std::string message;
	
	message = client.getMessage();
	while (message.find("\n") != std::string::npos)
	{
		std::string command = message.substr(0, message.find("\n"));
		message = message.substr(message.find("\n") + 1);
		client.setMessage(message);
		commands_parsing(client, command);
	}
	return (true);
}

void	Server::commands_parsing(Client &client, std::string input)
{
	std::deque<std::string>		list_arg;

	if (input.size() == 0)
		return ;
	list_arg = splitCommand(input);
	if (list_arg[0] == "PASS")
		checkPass(client, list_arg);
	if (client.getStatus() == true && list_arg[0] == "USER")
		checkUser(client, list_arg);
	else if (client.getStatus() == true && list_arg[0] == "NICK")
		checkNick(client, list_arg);
	if (client.getStatus() == false)
	{
		std::string	msg = NOTAUTHENTIFICATED;
		send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
		return ;
	}
	if (list_arg[0] == "PRIVMSG")
		checkPrivmsg(client, list_arg);
	else if (list_arg[0] == "JOIN")
		checkJoin(client, list_arg);
	else if (list_arg[0] == "MODE")
		checkMode(client, list_arg);
}

Server::Server(int port, std::string password)
{
	this->_password = password;
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(port);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(this->_server_socket, (struct sockaddr*)&this->_serverAddress,
		sizeof(this->_serverAddress));
	listen(this->getServerSocket(), 5);
	runtime();
}

Server::~Server()
{
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		close((*it)->getSocketFd());
		delete (*it);
	}
}

std::vector<Client*>&	Server::getListClient(void)
{
	return (this->_client_list);
}

int	Server::getServerSocket()
{
	return (this->_server_socket);
}

std::vector<Channel>&	Server::getListChannel(void)
{
	return (this->_channel_list);
}


void	Server::setPassword(std::string password)
{
	this->_password = password;
}

std::string		Server::getPassword(void)
{
	return (this->_password);
}

