/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:40:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/11/28 11:03:46 by ede-lang         ###   ########.fr       */
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
