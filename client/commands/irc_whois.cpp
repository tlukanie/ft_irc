/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_whois.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:16:13 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:16:41 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//WHOIS == DEBUG
void	irc_whois(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		//461??
		//add reply error
		return ;
	}
	if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '&')
	{
		std::string channelName = msg->getParams()[0];
		if (ts->channels.find(channelName) != ts->channels.end())
			ts->debugger.log += ts->channels[channelName]->print(ts->debugger.colour);
	}
	else
	{
		std::string nick = msg->getParams()[0];
		if (ts->nicks.find(nick) != ts->nicks.end())
			ts->debugger.log += ts->nicks[nick]->print(ts->debugger.colour);
	}
	//if not a channel
		//no such nick 401
	//if channel
		//402
}