/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:17:26 by ede-lang          #+#    #+#             */
/*   Updated: 2024/11/27 16:51:35 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <arpa/inet.h>
# include <stdlib.h>

# define BOTCONNEXION(pass, name) "PASS " + pass + "\r\nNICK " + name + "\r\nUSER " + name + "_ 0 * " + name + "\r\n"

class bot
{
	private:
		const std::string	_name;
		unsigned int		_port;
		std::string			_password;
		int					_bot_socket;
		sockaddr_in 		_botAddress;

		bot();

	public:
		bot(std::string name, unsigned int port, std::string pass);
		~bot();
		void runtime();

		int	getBotSocket() const;
};

#endif