/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:25:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/27 16:35:37 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define NB_MAX_CLIENTS 999

# include "Client.hpp"
# include <arpa/inet.h>
# include <cstring>
# include <iostream>
# include <vector>
# include <deque>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <poll.h>
# include <algorithm>
# include <csignal>
# include <cstdlib>
# include <errno.h>
# include <iomanip>
# include <string>
# include <ctime>
# include "Error.hpp"
# include "Channel.hpp"

class Client;
class Channel;

class Server
{
	private :
		int						_server_socket;
		sockaddr_in 			_serverAddress;
		std::vector<Client*>	_client_list;
		std::string				_password;
		std::vector<Channel>	_channel_list;

		void		runtime();
		void		initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1]);
		bool		add_client();
		void		read_all_clients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client);
		bool		process_commands(Client &client);

		std::string	checkPass(Client &client, std::deque<std::string> password);
		std::string	checkUser(Client &client, std::deque<std::string> data);
		std::string	checkNick(Client &client, std::deque<std::string> list_arg);
		std::string	checkPrivmsg(Client &client, std::deque<std::string> data);
		std::string	checkJoin(Client &client, std::deque<std::string> data);
		std::string	checkMode(Client &client, std::deque<std::string> data);
		std::string	checkInvite(Client &client, std::deque<std::string> data);
		std::string	checkTopic(Client &client, std::deque<std::string> data);
		std::string	checkKick(Client &client, std::deque<std::string> data);

		void		kick(Client &client, Channel &channel, std::string target, std::string msg);


		std::string sendToClient(Client &sender, std::string receiver, std::string msgToSend);
		std::string sendToChannel(Client &sender, std::string channel, std::string msgToSend);
		void		sendToAllClient(Client &client, std::string new_nickname);

		void		checkDeleteChannel(void);


	public :
		void		commands_parsing(Client &client, std::string commande);

		Client*		findClientByNick(std::string recipient);
		Channel*	findChannel(std::string channel);
		void		joinChannel(Client &client, Channel &channel) const;
		void		createChannel(Client &client, std::string name);

		void		sendToAll(Client &client);

		void		leaveAllChannel(Client &client);

		
		Server(int port, std::string password);
		~Server();

		int						getServerSocket();
		std::string				getPassword(void);
		std::vector<Channel>&	getListChannel(void);
		std::vector<Client*>&	getListClient(void);

		void					setPassword(std::string password);

		void					execMode(Client &client, std::deque<std::string> data, size_t &i, char token, char mode, Channel &channel);
};

std::deque<std::string>	splitCommand(std::string input);
std::deque<std::string>	parsingMultiArgs(std::string data);

#endif