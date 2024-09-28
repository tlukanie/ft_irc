/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_ping.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:53:02 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:53:30 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// PING
// https://modern.ircdocs.horse/#ping-message
// :IRCQ+ PONG net :IRCQ+
// Numeric Replies:

// ERR_NEEDMOREPARAMS (461)
// ERR_NOORIGIN (409)
void    irc_ping(Message* msg, struct s_server *ts)
{
	std::string	reply;

	//this check may not be needed
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	reply = "PONG " +  ts->users[msg->getSD()]->getNick() + " :" + msg->getParams()[0];
	send_reply(ts, msg->getSD(), NULL, reply);
}