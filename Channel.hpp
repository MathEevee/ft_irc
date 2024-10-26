/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:21:48 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/26 17:14:38 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <deque>
# include "Client.hpp"
# include "Error.hpp"

class Channel
{
	private :
		std::deque<Client>	_list_client;
		std::string			_name;
		bool				_mode_i;
		bool				_mode_t;
		bool				_mode_k;
		std::deque<Client>	_mode_o;
		bool				_mode_l;
		std::deque<Client>	_invite_list;
	public :
		Channel(std::string name, Client &new_client);
		~Channel();
		
		std::string			getName(void);
		void				setName(std::string name);
		
		void				addClient(Client &new_client);

		std::string			sendAllClient(Client &sender, std::string msg);
		Client*				findClientByNick(std::string sender, std::deque<Client> list);
};

#endif