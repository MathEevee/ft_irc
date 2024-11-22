/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:40:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/22 17:13:08 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <csignal>
# include "Server.hpp"

void	handler(int num)
{
	(void) num;
}

int main(int ac, char **av)
{
	if (ac != 3){
		std::cerr << "Error: invalid number of argument!" << std::endl; return (0);}
	signal(SIGINT, handler);
	Server	serv(std::atoi(av[1]), av[2]);
    close(serv.getServerSocket());
}
