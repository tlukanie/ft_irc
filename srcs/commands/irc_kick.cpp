/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_kick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:08:32 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:09:00 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//KICK
//https://modern.ircdocs.horse/#kick-message
//  KICK #Finnish John :Speaking English
//    :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
void	irc_kick(Message* msg, struct s_server *ts)
{
	std::string reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	// if user exists and if user not in channel, join user to channel
	if (msg->getParams().size() < 2)
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string					channelName = msg->getParams()[0];
	std::vector<std::string>	nicks = ok_split(msg->getParams()[1], ',');
	std::string reason;
	//403
	if (!isChannel(ts, channelName))
	{
		err_nosuchchannel_403(ts, msg->getSD(), channelName);
		return ;
	}
	//442
	if (!(ts->channels[channelName]->hasUser(msg->getSD())))
	{
		err_notonchannel_442(ts, msg->getSD(), channelName);
		return ;
	}
	//482
	if (!ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	for (size_t i = 0; i < nicks.size(); i++)
	{
		//401
		if (ts->nicks.find(nicks[i]) == ts->nicks.end())
		{
			err_nosuchnick_401(ts, msg->getSD(), nicks[i]);
			continue ;
		}
		//441
		if (!(ts->channels[channelName]->hasUser(ts->nicks[nicks[i]]->getSD())))
		{
			err_usernotinchannel_441(ts, msg->getSD(), nicks[i], channelName);
			continue ;
		}
		reply = "KICK " + channelName + " " + nicks[i];
		if (msg->getParams().size() > 2)
			reply += " :" + msg->getParams()[2];
		send_reply_channel(ts, channelName, ts->users[msg->getSD()], reply);
		remove_user_from_channel(ts, ts->nicks[nicks[i]], ts->channels[channelName]);
	}
}