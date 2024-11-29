/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:36:54 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/29 12:45:11 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iomanip>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

class Client
{
	private :
		std::string	_ip;
		std::string	_username;
		std::string	_real_name;
		std::string	_nickname;
		int			_status;
		int			_socket_fd;
		std::string	_message_buffer;
		bool		_disconnected;

	public :
		std::string		send_msg(std::string msg_error);

		Client();
		Client(int socket_fd, std::string ip);
		~Client();
		Client(const Client &obj);

		Client		&operator=(const Client &obj);

		void		setUsername(std::string username);
		void		setRealName(std::string real_name);
		void		setNickname(std::string nickname);
		void		setStatus(int status);
		void		setSocketFd(int socket_fd);
		void		setMessage(std::string message);
		void		setDisconnected(bool status);

		std::string	getIp(void);
		std::string	getUsername(void);
		std::string	getRealName(void);
		std::string	getNickname(void);
		int			getStatus(void);
		int			getSocketFd(void);
		std::string	getMessage(void);
		bool		getDisconnected(void);
};

#endif