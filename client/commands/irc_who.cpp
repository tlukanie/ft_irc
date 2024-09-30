/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_who.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:14:45 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 16:15:14 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//WHO
// https://modern.ircdocs.horse/#who-message
void	irc_who(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string	mask = msg->getParams()[0];
	if (mask[0] == '#' || mask[0] == '&')
	{
		if (isChannel(ts, mask))
		{
			for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(mask); it != ts->channel2user.upper_bound(mask); it++)
			{
				rpl_whoreply_352(ts, ts->users[msg->getSD()]->getNick(), mask, it->second->getNick());
			}
		}
	}
	else
	{
		if (isNick(ts, mask))
		{
			rpl_whoreply_352(ts, ts->users[msg->getSD()]->getNick(), "", mask);
		}
	}
	rpl_endofwho_315(ts, ts->users[msg->getSD()]->getNick(), mask);
}