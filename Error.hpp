/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:16:46 by matde-ol          #+#    #+#             */
/*   Updated: 2024/10/23 17:21:57 by matde-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# define ERR_ALREADYREGISTRED(name) "127.0.0.1 462 " + name + ":You may not reregister\r\n"
# define ERR_PASSWDMISMATCH(name) "127.0.0.1 464 " + name + ":Password incorrect\r\n"
# define ERR_NEEDMOREPARAMS(name, cmd) "127.0.0.1 461 " + name + ":" + cmd + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(name, cmd) "127.0.0.1 460 " + name + ":" + cmd + " :Too many parameters\r\n"
# define AUTHENTIFICATED(name) ":127.0.0.1 " + name + ": Your are authentificated.\r\n"

#endif