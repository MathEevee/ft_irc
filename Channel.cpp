/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:32:23 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/22 17:37:21 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client &new_client)
{
	setName(name);
	addClient(new_client);
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