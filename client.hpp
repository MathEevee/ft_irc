/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:36:54 by matde-ol          #+#    #+#             */
/*   Updated: 2024/09/29 11:51:56 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
	private :
		int	_socket_fd;
		int	_status;
	public :
		int		get_socket_fd(void);
		int		get_status(void);
		void	set_socket_fd(int socket_fd);
		void	set_status(int status);

		Client(int socket_fd);
		~Client();
};

#endif