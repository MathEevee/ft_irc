/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:21:48 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/29 12:45:34 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <deque>
# include "Client.hpp"
# include "Server.hpp"
# include "Define.hpp"

class Channel
{
	private :
		std::deque<Client*>	_list_client;
		std::string			_name;

		bool				_mode_i;
		std::deque<Client*>	_invite_list;

		bool				_mode_t;
		std::string			_topic;

		bool				_mode_k;
		std::string			_password;

		std::deque<Client*>	_mode_o;

		bool				_mode_l;
		size_t				_nbr_client;
	public :
		Channel();
		Channel(std::string name, Client &new_client);
		~Channel();
		
		std::string				getName(void);
		bool					getModeK(void);
		bool					getModeI(void);
		bool					getModeL(void);
		bool					getModeT(void);
		std::string				getPassword(void);
		std::deque<Client*>&	getList(void);
		size_t					getNbrClient(void);
		std::deque<Client*>&	getAllClient(void);
		std::deque<Client*>&	getClientOp(void);
		std::string				getAllMode(void);
		std::string				getAllUser(void);
		std::string				getTopic(void);

		void					setName(std::string name);
		void					setTopic(std::string topic);
		void					setPassword(std::string password);
		void					setNbrClient(size_t nbr);
		void					setModeL(bool l);
		void					setModeT(bool t);
		void					setModeI(bool i);
		void					setModeK(bool k);
		
		void					addClient(Client &new_client);
		void					removeClient(Client &client);
		void					deleteClient(Client &client, std::deque<Client*> &list);
		void					sendMsgJoin(Client &client);

		std::string				removeOp(Client &client, std::deque<std::string> data, size_t &i);
		std::string				addOp(Client &client, std::deque<std::string> data, size_t &i);
		
		std::string				sendAllClient(Client &sender, std::string msg);
		std::string				sendAllClientMsg(Client &sender, std::string msg);
		Client*					findClientByNick(std::string sender, std::deque<Client*> &list);

		void					print(std::string sender, std::deque<Client*> &list, std::string msg);


		std::string				execModeI(Client &client, char token);
		std::string				execModeT(Client &client, char token);
		std::string				execModeK(Client &client, std::deque<std::string> data, size_t &i, char token);
		std::string				execModeL(Client &client, std::deque<std::string> data, size_t &i, char token);
};

#endif