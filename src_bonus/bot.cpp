/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:19:34 by ede-lang          #+#    #+#             */
/*   Updated: 2024/11/27 16:51:15 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

bot::bot(std::string name, unsigned int port, std::string pass)
: _name(name), _port(port), _password(pass)
{
	this->_bot_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_botAddress.sin_family = AF_INET;
	this->_botAddress.sin_port = htons(port);
	this->_botAddress.sin_addr.s_addr = INADDR_ANY;
	bind(this->_bot_socket, (struct sockaddr*)&this->_botAddress,
		sizeof(this->_botAddress));
	listen(this->_bot_socket, 5);
}

bot::~bot() {}

void bot::runtime()
{
	ssize_t size;
	char	buffer[1024];

	std::string msg = BOTCONNEXION(this->_password, this->_name);

	send(this->getBotSocket(), msg.c_str(), msg.size(), 0);
	while (1)
	{
		size = recv(this->getBotSocket(), buffer, sizeof(buffer) - 1, 0);
		if (size > 0)
		{
			std::cout << buffer << std::endl;
		}

	}
}


//GETTERS
int	bot::getBotSocket() const { return (this->_bot_socket);}

