/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:21:48 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/22 17:34:48 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "Client.hpp"

class Channel
{
	private :
		std::vector<Client>	_list_client;
		std::string			_name;
	public :
		Channel(std::string name, Client &new_client);
		~Channel();
		
		std::string			getName(void);
		void				setName(std::string name);
		
		void				addClient(Client &new_client);
};

#endif