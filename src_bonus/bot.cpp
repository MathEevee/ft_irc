/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:19:34 by ede-lang          #+#    #+#             */
/*   Updated: 2024/11/29 12:41:53 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

int signal_num;

void	handler(int num)
{
	signal_num = num;
}

bot::bot(std::string name, unsigned int port, std::string pass)
: _name(name), _port(port), _password(pass)
{
	this->_bot_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_botAddress.sin_family = AF_INET;
	this->_botAddress.sin_port = htons(port);
	this->_botAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(this->_bot_socket, (struct sockaddr *)&this->_botAddress, sizeof(struct sockaddr));
}

bot::~bot() {}

bool bot::exchangemsg(std::string msg)
{
	char	buffer[1024];
	size_t	size;

	send(this->getBotSocket(), msg.c_str(), msg.size(), 0);
	if(signal_num == SIGPIPE)
		return (false);
	size = recv(this->getBotSocket(), buffer, sizeof(buffer) - 1, 0);
	buffer[size] = 0;
	std::cout << buffer << std::endl;
	return (true);
}

bool bot::connectServer()
{
	signal(SIGPIPE, handler);

	signal_num = 0;

	if (!exchangemsg(PASS(this->_password)))
		return (false);

	if (!exchangemsg(USER(this->_name)))
		return (false);
	
	if (!exchangemsg(NICK(this->_name)))
		return (false);
	
	std::string channel = "#salut";
	if (!exchangemsg(JOIN(channel)))
		return (false);

	return (true);
}

void bot::runtime()
{
	ssize_t size;
	char	buffer[1024];
	if (!bot::connectServer())
		return ;
	std::string message = "";
	std::deque<std::string> list_arg;


	signal(SIGINT, handler);

	while (signal_num != SIGINT && signal_num != SIGPIPE)
	{
		do
		{
			size = recv(this->getBotSocket(), buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
			if (size == -1)
				break;
			buffer[size] = 0;
			message = message + buffer;
		} while (size == 1024);
		if (message != "")
		{
			std::cout << message <<std::endl;
			list_arg = splitCommand(message);
			if (list_arg[1] == "PRIVMSG")
			{
				this->send_msg(list_arg);
			}
		}
		message = "";
	}
}

//GETTERS
int	bot::getBotSocket() const { return (this->_bot_socket);}

bool bot::send_msg(std::deque<std::string> list_arg)
{
	std::string dest;
	std::string msg;

	if (list_arg[2][0] == '#')
		dest = list_arg[2];
	else
		dest = list_arg[0].substr(1, list_arg[0].find('!') - 1);
	
	msg = list_arg[1] + " " + dest + " : Do you know I'm a bot ? \r\n";
	send(this->getBotSocket(), msg.c_str(), msg.size(), 0);
	
	return (true);
}
