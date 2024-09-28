/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_pong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:54:22 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:55:05 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// PONG
// https://modern.ircdocs.horse/#pong-message
void    irc_pong(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
}