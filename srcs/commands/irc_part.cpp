/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_part.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:58:59 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:23:15 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// PART
//https://modern.ircdocs.horse/#part-message
void	irc_part(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::vector<std::string>	channelNames = ok_split(msg->getParams()[0], ',');
	std::string					reason;
	if (msg->getParams().size() > 1)
		reason = msg->getParams()[1];
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		//if the channel does not exist
		if (ts->channels.find(channelNames[i]) == ts->channels.end())
		{
			err_nosuchchannel_403(ts, msg->getSD(), channelNames[i]);
			continue ;
		}
		if (!(ts->channels[channelNames[i]]->hasUser(msg->getSD())))
		{
			err_notonchannel_442(ts, msg->getSD(), channelNames[i]);
			continue ;
		}
		ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(msg->getSD()) + "] Leaving channel:", channelNames[i], MYDEBUG);
		reply += "PART " + channelNames[i] + " :";
		reply += (reason.size() ? reason : std::string(ts->users[msg->getSD()]->getNick() + " leaving channel " + channelNames[i]));
		//send reply to channel
		send_reply_channel(ts, channelNames[i], ts->users[msg->getSD()], reply);
		remove_user_from_channel(ts, ts->users[msg->getSD()], ts->channels[channelNames[i]]);
	}
}