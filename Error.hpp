/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:16:46 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/24 17:01:13 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# define ERR_ALREADYREGISTRED "127.0.0.1 462 :You may not reregister\r\n"
# define ERR_PASSWDMISMATCH "127.0.0.1 464 :Password incorrect\r\n"
# define ERR_NEEDMOREPARAMS(cmd) "127.0.0.1 461 " + cmd + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(cmd) "127.0.0.1 460 " + cmd + " :Too many parameters\r\n"
# define AUTHENTIFICATED(name) ":127.0.0.1 " + name + ":Your are authentificated\r\n"
# define ERR_NOTREGISTERED ":127.0.0.1 451 :You have not registered\r\n"
# define ERR_NICKNAMEINUSE(name) ":127.0.0.1 433 " + name + ":Nickname is already in use\r\n"
# define ERR_NONICKNAMEGIVEN "127.0.0.1 431 :No nickname given\r\n"
# define ERR_NOSUCHNICK(name) ":127.0.0.1 401 " + name + ":No such nick/channel\r\n"

#endif