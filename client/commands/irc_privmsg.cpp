/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:11:13 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:11:47 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there
//not sending messages to chanops only, yet...

void	irc_privmsg(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_norecipient_411(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	if (msg->getParams().size() < 2)
	{
		err_notexttosend_412(ts, msg->getSD());
		return ;
	}
	std::vector<std::string>	targets = ok_split(msg->getParams()[0], ',');
	std::string					text = msg->getParams()[1];

	for (size_t i = 0; i < targets.size(); i++)
	{
		reply = "PRIVMSG " + targets[i] + " :" + text;
		if (!targets[i].size())
		{
			err_norecipient_411(ts, msg->getSD(), msg->getCommand());
			continue ;
		}
		if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '&')
		{
			//403
			if (ts->channels.find(targets[i]) == ts->channels.end())
			{
				err_nosuchchannel_403(ts, msg->getSD(), targets[i]);
				continue ;
			}
			send_reply_channel(ts, targets[i], ts->users[msg->getSD()], reply);
		}
		else
		{
			//401
			if (ts->nicks.find(targets[i]) == ts->nicks.end())
			{
				err_nosuchnick_401(ts, msg->getSD(), targets[i]);
				continue ;
			}
			send_reply(ts, ts->nicks[targets[i]]->getSD(), ts->users[msg->getSD()], reply);
		}
	}
}