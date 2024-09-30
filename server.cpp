/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:26:06 by matde-ol          #+#    #+#             */
/*   Updated: 2024/09/30 17:47:04 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	Server::getServSocket()
{
	return (this->_serv_socket);
}

void	Server::set_fds(struct pollfd fds[NB_MAX_CLIENTS + 1])
{
	fds[0].fd = this->getServSocket();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int	i = 1;
	for (std::vector<Client>::iterator it = _tab_client.begin(); it != _tab_client.end(); it++)
	{
		fds[i].fd = it->get_socket_fd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
		i++;
	}
	std::cout << i << std::endl;
}

bool	Server::add_client()
{
	int clientSocket = accept(this->getServSocket(), nullptr, nullptr);
	if (clientSocket != -1)
	{
		if (this->_tab_client.size() >= NB_MAX_CLIENTS)
		{
			close(clientSocket);
			std::cout << "Too many clients" << std::endl;
		}
		else
		{
			Client	new_client(clientSocket);
			this->_tab_client.push_back(new_client);
			return (true);
		}
	}
	return (false);
}

void	Server::read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
{
	int	i = 1;
	ssize_t size;
	char	buffer[1024];
	for (std::vector<Client>::iterator it = this->_tab_client.begin(); it != this->_tab_client.end() - new_client; it++)
	{
		std::string	message;
		if ((fds[i].revents & POLLIN) != 0)
		{
			do
			{
				size = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				buffer[size] = 0;
				message = message + buffer;
			} while (size == 1024);
			//parsing commande
			std::cout << "client : " << fds[i].fd << " message : " << message << std::endl;
			message = "";
		}
		else if ((fds[i].revents & POLLHUP) != 0)
		{}
		i++;
	}
}

void	Server::runtime()
{
	struct pollfd fds[NB_MAX_CLIENTS + 1];

	// int clientSocket = accept(this->getServSocket(), nullptr, nullptr);

    char buffer[1024] = { 0 };
	while (1)
	{
		bool	new_client = false;
		set_fds(fds);
		std::cout << _tab_client.size() + 1 << std::endl;
		int	nb_fd = poll(fds, _tab_client.size() + 1, -1);
		std::cout << "poll released" << std::endl;
		std::cout << nb_fd << std::endl;
		if ((fds[0].revents & POLLIN) != 0)
		{
			new_client = add_client();
		}
		read_all_clients(fds, new_client);
	}
}

Server::Server(int port)
{
	this->_serv_socket = socket(AF_INET, SOCK_STREAM, 0);

	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(port);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(this->_serv_socket, (struct sockaddr*)&this->_serverAddress,
		sizeof(this->_serverAddress));
	listen(this->getServSocket(), 5);
	runtime();
}


Server::~Server()
{}
