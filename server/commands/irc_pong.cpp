/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_pong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:54:22 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:23:21 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// PONG
// https://modern.ircdocs.horse/#pong-message
void	irc_pong(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
}