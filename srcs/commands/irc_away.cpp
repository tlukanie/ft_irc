/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_away.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:09:46 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:10:09 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//AWAY
//https://modern.ircdocs.horse/#away-message
void	irc_away(Message* msg, struct s_server *ts)
{
	//not supported yet
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		ts->users[msg->getSD()]->setAwayMessage("");
		//reply unwaway
		send_reply(ts, msg->getSD(), NULL, "305 " + ts->users[msg->getSD()]->getNick() + " :You are no longer marked as being away");
	}
	else if (msg->getParams().size() == 1)
	{
		ts->users[msg->getSD()]->setAwayMessage(msg->getParams()[0]);
		//reply nowaway
		send_reply(ts, msg->getSD(), NULL, "306 " + ts->users[msg->getSD()]->getNick() + " :You have been marked as being away");
	}
}