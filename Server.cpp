/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:26:06 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/24 15:18:44 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::connexionFull(void)
{
	
}

std::deque<std::string>	splitCommand(std::string input)
{
	std::deque<std::string>		tab;
	std::string					arg;
	bool						end_command = false;


	if (input[input.size() - 1] == '\r')
	{
		input = input.substr(0, input.size() - 1);
	}
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
	{
		if (arg[arg.size()] == '\r')
			arg = arg.substr(0, arg.size() - 1);
		tab.push_back(arg);
	}
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
	for (std::vector<Client>::iterator it = _client_list.begin(); it != _client_list.end(); it++)
	{
		fds[i].fd = it->getSocketFd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
		i++;
	}
}

Client*		Server::findClientByNick(std::string recipient)
{
	for (std::vector<Client>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		if (it->getNickname() == recipient)
			return (&(*it));
	}
	return (NULL);
}


bool	Server::add_client()
{
	int clientSocket = accept(this->getServerSocket(), NULL, NULL);
	if (clientSocket != -1)
	{
		if (this->_client_list.size() >= NB_MAX_CLIENTS)
		{
			close(clientSocket);
			std::cout << "Too many clients" << std::endl;
		}
		else
		{
			Client	new_client(clientSocket);
			this->_client_list.push_back(new_client);
			std::cout << "New client connected." << std::endl;
			return (true);
		}
	}
	return (false);
}

void	Server::read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
{
	int		i = 1;
	ssize_t size;
	char	buffer[1024];

	for (std::vector<Client>::iterator it = this->_client_list.begin(); it != this->_client_list.end() - new_client;)
	{
		std::string	message = it->getMessage();

		if ((fds[i].revents & POLLIN) != 0)
		{
			do
			{
				size = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (size == 0)
				{
					it->setDisconnected(true);
					this->sendToAll(*it);
				}
				buffer[size] = 0;
				message = message + buffer;
				it->setMessage(message);
			} while (size == 1024);

			this->process_commands(*it);
			i++;
		}

		if ((*it).getDisconnected() == false)
			it++;
		else
			it = this->_client_list.erase(it);
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

	list_arg = splitCommand(input);
	if (list_arg[0] == "PASS")
		checkPass(client, list_arg);
	if (list_arg[0] == "USER")
		checkUser(client, list_arg);
	else if (list_arg[0] == "NICK")
		checkNick(client, list_arg);
	// //add checkpoint to check user initialized, not initialized send error & stop
	// if (command == "PRIVMSG")
	// 	result = checkPrivmsg(client, input.substr(input.find(' ') + 1));

}

void	Server::sendToAll(Client &client)
{
	//for() loop all chann to send in all channel disconnected
	std::string all_message = client.getNickname() + ": " + "disconnected" + "\r\n";
	// send(this->getServerSocket(), all_message.c_str(), all_message.size(), 0);
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
	for (std::vector<Client>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		close(it->getSocketFd());
	}
}

int	Server::getServerSocket()
{
	return (this->_server_socket);
}

void	Server::setPassword(std::string password)
{
	this->_password = password;
}

std::string		Server::getPassword(void)
{
	return (this->_password);
}

