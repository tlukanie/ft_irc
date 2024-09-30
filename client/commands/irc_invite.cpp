/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_invite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:07:02 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:07:43 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//INVITE
// https://modern.ircdocs.horse/#invite-message
// INVITE user123 #test
// FD:5	:IRCQ+ 341 net user123 #test
// FD:5	:user123!net@127.0.0.1 JOIN :#test
// FD:6	:user123!net@127.0.0.1 JOIN :#test
// FD:6	:IRCQ+ 332 user123 #test :
// FD:6	:IRCQ+ 353 user123 = #test :@net user123
// FD:6	:IRCQ+ 366 user123 #test :End of Names list

void	irc_invite(Message* msg, struct s_server *ts)
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
	std::string nick = msg->getParams()[0];
	std::string channelName = msg->getParams()[1];
	//401
	if (ts->nicks.find(nick) == ts->nicks.end())
	{
		err_nosuchnick_401(ts, msg->getSD(), nick);
		return ;
	}
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
	//443
	if (ts->channels[channelName]->hasUser(ts->nicks[nick]->getSD()))
	{
		err_useronchannel_443(ts, msg->getSD(), nick, channelName);
		return ;
	}
	//482
	if ((ts->channels[channelName]->getModeFlags() & CHANNEL_INVITE) && !ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	if ((ts->channels[channelName]->getModeFlags() & CHANNEL_LIMIT) && ts->channels[channelName]->getUsers() >= ts->channels[channelName]->getChannelLimit())
	{
		err_channelisfull_471(ts, msg->getSD(), channelName);
		return ;
	}
	//341
	rpl_inviting_341(ts, msg->getSD(), nick, channelName);
	//invite message
	reply = "INVITE ";
	reply += nick + " ";
	reply += channelName;
	send_reply(ts, ts->nicks[nick]->getSD(), ts->users[msg->getSD()], reply);
	// not sure about the part below?
	reply = "JOIN :" + channelName;
	add_user_to_channel(ts, ts->nicks[nick], ts->channels[channelName]);
	send_reply_channel(ts, channelName, ts->nicks[nick], reply);
	// send_reply(ts, ts->nicks[nick]->getSD(), ts->nicks[nick], reply);
}