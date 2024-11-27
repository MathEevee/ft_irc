/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:15:35 by ede-lang          #+#    #+#             */
/*   Updated: 2024/11/27 16:47:20 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

bool verif_param(char *argv[])
{
	//TODO param parsing
	(void) argv;
	return (true);
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::cout << "usage : ./bot <botname> <port> <pass>" << std::endl;
		return (1);
	}
	if(verif_param(argv))
	{
		bot robot(argv[1], atoi(argv[2]), argv[3]);
		robot.runtime();
	}

	return 0;
}
