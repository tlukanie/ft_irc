/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:57:22 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:58:05 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// JOIN
// https://modern.ircdocs.horse/#join-message
void    irc_join(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	//special case
	if (msg->getParams()[0] == "0")
	{
		//leave all channels
		for (std::multimap<std::string, Channel*>::iterator it = ts->user2channel.lower_bound(ts->users[msg->getSD()]->getNick()); it != ts->user2channel.upper_bound(ts->users[msg->getSD()]->getNick()); /*Iterating in the loop*/)
		{
			std::multimap<std::string, Channel*>::iterator temp;
			temp = it++;
			reply += "PART " + temp->second->getChannelName() + " :leaving";
			//send reply to channel
			send_reply_channel(ts, temp->second->getChannelName(), ts->users[msg->getSD()], reply);
			remove_user_from_channel(ts, ts->users[msg->getSD()], ts->channels[temp->second->getChannelName()]);
		}
		return ;
	}
	std::vector<std::string>	channelNames = ok_split(msg->getParams()[0], ',');
	std::vector<std::string>	keys;
	if (msg->getParams().size() > 1)
		keys =  ok_split(msg->getParams()[1], ',');
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		//check if the channel name valid
		if (channelNames[i].size() < 2 || (channelNames[i][0] != '#' && channelNames[i][0] != '&'))
		{
			err_badchanmask_476(ts, msg->getSD(), channelNames[i]);
			continue ;
		}
		//if the channel does not exist
		if (ts->channels.find(channelNames[i]) == ts->channels.end())
		{
			try
			{
				ts->channels[channelNames[i]] = new Channel(channelNames[i], msg->getSD());
				ok_debugger(&(ts->debugger), INFO, "Creating channel: ", channelNames[i], MYDEBUG);
			}
			catch(const std::exception& e)
			{
				ok_debugger(&(ts->debugger), ERROR, "Could not create channel: " + channelNames[i] + ", because: ", e.what(), MYDEBUG);
				return ;
			}
		}
		else
		{
			//check if user can join the channel
			if (ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_INVITE)
			{
				err_inviteonlychan_473(ts, msg->getSD(), channelNames[i]);
				continue ;
			}
			if ((ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_LIMIT) && ts->channels[channelNames[i]]->getUsers() >= ts->channels[channelNames[i]]->getChannelLimit())
			{
				err_channelisfull_471(ts, msg->getSD(), channelNames[i]);
				continue ;
			}
			if ((ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_KEY))
			{
				if (keys.size() <= i || keys[i] != ts->channels[channelNames[i]]->getKey())
				{
					err_badchannelkey_475(ts, msg->getSD(), channelNames[i]);
					continue ;
				}
			}
		}
		add_user_to_channel(ts, ts->users[msg->getSD()], ts->channels[channelNames[i]]);
		//send everyone join message
		ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(msg->getSD()) + "] Joining channel:", channelNames[i], MYDEBUG);
		reply = "JOIN :" + channelNames[i];
		send_reply_channel(ts, channelNames[i], ts->users[msg->getSD()], reply);
		//332
		if (ts->channels[channelNames[i]]->getTopic().size())
		{
			rpl_topic_332(ts, msg->getSD(), channelNames[i]);
		}
		rpl_namreply_353(ts, msg->getSD(), channelNames[i]);
		rpl_endofnames_366(ts, msg->getSD(), channelNames[i]);
	}
}