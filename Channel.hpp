/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:21:48 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/30 18:00:48 by matde-ol         ###   ########.fr       */
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
		std::deque<Client>	_invite_list;
		bool				_mode_t;
		bool				_mode_k;
		std::string			_password;
		std::deque<Client>	_mode_o;
		bool				_mode_l;
		size_t				_nbr_client;
	public :
		Channel(std::string name, Client &new_client);
		~Channel();
		
		std::string			getName(void);
		bool				getModeK(void);
		bool				getModeI(void);
		bool				getModeL(void);
		std::string			getPassword(void);
		std::deque<Client>&	getList(void);
		size_t				getNbrClient(void);
		std::deque<Client>&	getAllClient(void);
		std::deque<Client>&	getClientOp(void);

		void				setName(std::string name);
		void				setPassword(std::string password);
		void				setNbrClient(size_t nbr);
		void				setModeL(bool l);
		
		void				addClient(Client &new_client, std::string msg);
		void				removeClient(Client &client);// remove all list
		void				deleteClient(Client &client, std::deque<Client> &list);
		

		std::string			sendAllClient(Client &sender, std::string msg);
		Client*				findClientByNick(std::string sender, std::deque<Client> list);
};

#endif