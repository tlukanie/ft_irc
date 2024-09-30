/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_notice.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:12:58 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:13:39 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//NOTICE
//https://modern.ircdocs.horse/#notice-message

void	irc_notice(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		//411
		return ;
	}
	if (msg->getParams().size() < 2)
	{
		//412
		return ;
	}
	std::vector<std::string>	targets = ok_split(msg->getParams()[0], ',');
	std::string					text = msg->getParams()[1];

	for (size_t i = 0; i < targets.size(); i++)
	{
		reply = "NOTICE " + targets[i] + " :" + text;
		if (!targets[i].size())
		{
			//411
			continue ;
		}
		if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '&')
		{
			//403
			if (ts->channels.find(targets[i]) == ts->channels.end())
			{
				continue ;
			}
			send_reply_channel(ts, targets[i], ts->users[msg->getSD()], reply);
		}
		else
		{
			//401
			if (ts->nicks.find(targets[i]) == ts->nicks.end())
			{
				continue ;
			}
			send_reply(ts, ts->nicks[targets[i]]->getSD(), ts->users[msg->getSD()], reply);
		}
	}
}