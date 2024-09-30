/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:47:20 by matde-ol          #+#    #+#             */
/*   Updated: 2024/09/30 17:41:04 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int socket_fd)
{
	this->set_socket_fd(socket_fd);
	this->set_status(0);
}

int	Client::get_status()
{
	return (this->_status);
}

int	Client::get_socket_fd()
{
	return (this->_socket_fd);
}

void	Client::set_socket_fd(int socket_fd)
{
	this->_socket_fd = socket_fd;
}

void	Client::set_status(int status)
{
	this->_status = status;
}

Client::~Client()
{}
