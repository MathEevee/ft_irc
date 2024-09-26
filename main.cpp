/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:40:59 by matde-ol          #+#    #+#             */
/*   Updated: 2024/09/26 18:50:02 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Bad argument" << std::endl;
		return (0);
	}

	int	ip = std::atoi(av[1]);

	int serv_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(ip);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serv_socket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));
    listen(serv_socket, 5);

    int clientSocket
        = accept(serv_socket, nullptr, nullptr);

    // recieving data
    char buffer[1024] = { 0 };
	while (recv(clientSocket, buffer, sizeof(buffer), 0) != 0)
	{
    	std::cout << "Message from client: " << buffer << std::endl;
		write(clientSocket, "salut\n", 6);
		int i = 0;
		while (i <= 1024)
		{
			buffer[i] = '\0';
			i++;
		}
	}
    // closing the socket.
    close(serv_socket);
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