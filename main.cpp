/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:40:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/09/30 15:48:15 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Bad argument" << std::endl;
		return (0);
	}
	
	Server	serv(std::atoi(av[1]));


    // closing the socket.
    close(serv.getServSocket());
	// close(serv_socket);
}

// int main()
// {
//     // creating socket
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

//     // specifying address
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     // sending connection request
//     connect(clientSocket, (struct sockaddr*)&serverAddress,
//             sizeof(serverAddress));

//     // sending data
// 	std::string	message;
// 	while (getline(std::cin, message))
// 	{
//     	send(clientSocket, message.c_str(), message.size(), 0);
// 		message = "";
// 	}
//     // closing socket
//     close(clientSocket);

//     return 0;
// }