/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:22 by okraus            #+#    #+#             */
/*   Updated: 2024/09/26 09:46:40 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/ircserv.hpp"

// "<client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
//RPL_WHOREPLY (352)
void	ok_send_352(struct s_server *ts, std::string client, std::string channelName, std::string nick)
{
	std::string	reply;
	std::string channel = channelName;
	reply = "352 ";
	reply += client + " ";
	if (!channel.size())
	{
		std::cout << "nick: " << nick << std::endl; 
		std::multimap<std::string, Channel*>::iterator it = ts->user2channel.find(nick);
		if (it != ts->user2channel.end())
			channel += it->second->getChannelName();
		else
			channel += "*";
		for (std::multimap<std::string, Channel*>::iterator iter = ts->user2channel.begin(); iter != ts->user2channel.end(); iter++)
			std::cout << "nick: " << it->first << " | chennel: " << it->second->getChannelName() << std::endl;
	}
	reply += channel + " ";
	reply += ts->nicks[nick]->getUserName() + " ";
	// if (ts->nicks[nick]->getIP()[0] == ':')
	// 	reply += "0";
	// reply += ts->nicks[nick]->getIP() + " ";
	reply += nick + " ";
	reply += ts->servername + " ";
	reply += nick + " ";
	reply += "H";
	if (channel != "*" && ts->channels[channel]->isOperator(ts->nicks[nick]->getSD()))
		reply += "@";
	reply += " ";
	reply += ":1 " + ts->nicks[nick]->getRealName();
	send_reply(ts, ts->nicks[client]->getSD(), NULL, reply);
}

//RPL_ENDOFWHO (315)
void	ok_send_315(struct s_server *ts, std::string client, std::string mask)
{
	std::string	reply;
	reply = "315 ";
	reply += client + " ";
	reply += mask + " ";
	reply += ":End of WHO list";
	send_reply(ts, ts->nicks[client]->getSD(), NULL, reply);
}

//ERR_UNKNOWNCOMMAND (421)
void	ok_send_421(struct s_server *ts, unsigned short sd, std::string command)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "421 ";
	reply += client + " ";
	reply += command + " ";
	reply += ":Unknown command";
	send_reply(ts, sd, NULL, reply);
}
