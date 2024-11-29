/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:15:35 by ede-lang          #+#    #+#             */
/*   Updated: 2024/11/29 12:50:54 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

bool verif_param(char *argv[])
{
	for (int i = 0; argv[1][i]; i++)
		if (!isalnum(argv[1][i]))
			return (false);
	for (int i = 0; argv[2][i]; i++)
		if (!isdigit(argv[2][i]))
			return (false);
	return (true);
}

int main(int argc, char *argv[])
{
	if (argc != 4 || !verif_param(argv))
	{
		std::cout << "usage : ./bot <botname> <port> <pass>" << std::endl;
		return (1);
	}
	bot robot(argv[1], atoi(argv[2]), argv[3]);
	robot.runtime();
	close(robot.getBotSocket());
	return 0;
}
