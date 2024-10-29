/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:16:46 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/29 18:12:47 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# define ERR_ALREADYREGISTRED "127.0.0.1 462 :You may not reregister\r\n"
# define ERR_PASSWDMISMATCH "127.0.0.1 464 :Password incorrect\r\n"
# define ERR_NEEDMOREPARAMS(cmd) "127.0.0.1 461 " + cmd + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(cmd) "127.0.0.1 460 " + cmd + " :Too many parameters\r\n"
# define AUTHENTIFICATED(name) ":127.0.0.1 " + name + " :Your are authentificated\r\n"
# define NOTAUTHENTIFICATED ":127.0.0.1 :Your are not authentificated\r\n"
# define ERR_NOTREGISTERED ":127.0.0.1 451 :You have not registered\r\n"
# define ERR_NICKNAMEINUSE(name) ":127.0.0.1 433 " + name + " :Nickname is already in use\r\n"
# define ERR_NONICKNAMEGIVEN "127.0.0.1 431 :No nickname given\r\n"
# define CHANGENICKNAME(name) "127.0.0.1 " + name + " :You have change your Nickname\r\n"
# define CHANGENICKNAMEFORALL(name, new_nickname) "127.0.0.1 " + name + " :Change his nickname to " + new_nickname + "\r\n"
# define SELECTNICKNAME(name) "127.0.0.1 " + name + " :You have choose your Nickname\r\n"
# define SELECTUSER(name) "127.0.0.1 " + name + " :You have choose your User and Realname\r\n"
# define ERR_NOSUCHNICK(name) ":127.0.0.1 401 " + name + " :No such nick/channel\r\n"
# define ERR_NOSUCHCHANNEL(name) ":127.0.0.1 403 " + name + " :No such channel\r\n"
# define ERR_NORECIPIENT(cmd) ":127.0.0.1 411 :No recipient given " + cmd + "\r\n"
# define ERR_NOTEXTTOSEND ":127.0.0.1 412 :No text to send\r\n"
# define ERR_CANNOTSENDTOCHAN(channel) ":127.0.0.1 404 " + channel + " :Cannot send to channel\r\n"
# define ERR_INVITEONLYCHAN(channel) ":127.0.0.1 473 " + channel + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(channel) ":127.0.0.1 475 " + channel + " :Cannot join channel (+k)\r\n"
# define ERR_CHANNELISFULL(channel) ":127.0.0.1 471 " + channel + " :Cannot join channel (+l)\r\n"

# define MSGSEND(nameSender, userNameSender, ipSender, nickReceiver, msg) ":" + nameSender + "!" + userNameSender + "@" + ipSender + " PRIVMSG " + nickReceiver + " " + msg + "\r\n"
# define MSGSEND(nameSender, userNameSender, ipSender, channel) ":" + nameSender + "!" + userNameSender + "@" + ipSender + " JOIN " + channel + "\r\n"

#endif