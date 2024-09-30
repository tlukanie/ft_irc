/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:04:19 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:05:01 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//TOPIC
// https://modern.ircdocs.horse/#topic-message
//TOPIC #test :Hello
//:user123!net@127.0.0.1 TOPIC #test :Hello

void	irc_topic(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	//send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text, int flags)
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string channelName = msg->getParams()[0];
	std::string topic;
	if (msg->getParams().size() > 1)
		topic = msg->getParams()[1];

	if (!isChannel(ts, channelName))
	{
		err_nosuchchannel_403(ts, msg->getSD(), channelName);
		return ;
	}
	if (!(ts->channels[channelName]->hasUser(msg->getSD())))
	{
		err_notonchannel_442(ts, msg->getSD(), channelName);
		return ;
	}
	if (msg->getParams().size() == 1)
	{
		if (ts->channels[channelName]->getTopic().size())
		{
			rpl_topic_332(ts, msg->getSD(), channelName);
			return ;
		}
		else
		{
			rpl_notopic_331(ts, msg->getSD(), channelName);
			return ;
		}
	}
	// setting topic
	//482
	if ((ts->channels[channelName]->getModeFlags() & CHANNEL_TOPIC) && !ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	reply = "TOPIC " + channelName + " :" + topic;
	ts->channels[channelName]->setTopic(topic);
	send_reply_channel(ts, channelName, ts->users[msg->getSD()], reply);
}