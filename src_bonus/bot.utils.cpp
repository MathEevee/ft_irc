/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:40:51 by ede-lang          #+#    #+#             */
/*   Updated: 2024/11/28 11:43:47 by ede-lang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

std::deque<std::string>	splitCommand(std::string input)
{
	std::deque<std::string>		tab;
	std::string					arg;
	bool						end_command = false;
	int							cmd = 0;

	if (input[input.size() - 1] == '\r')
		input = input.substr(0, input.size() - 1);
	for (std::string::iterator it = input.begin(); it != input.end(); it++)
	{
		if (*it == ' ' && end_command == false)
		{
			if (arg.size() != 0)
			{
				tab.push_back(arg);
				arg = "";
			}
		}
		else if (*it == ':')
		{
			cmd++;
			if (cmd == 2)
				end_command = true;
		}

		if (*it != ' ' || end_command == true)
			arg += *it;
	}
	if (arg.size() != 0)
		tab.push_back(arg);
	return (tab);
}