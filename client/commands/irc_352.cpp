/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_352.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:11:13 by tlukanie          #+#    #+#             */
/*   Updated: 2024/10/04 15:02:57 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there
//not sending messages to chanops only, yet...

void	irc_352(Message* msg, struct s_client *tc)
{
	std::string	nick;

	if (msg->getParams().size() < 4)
	{
		// err_notexttosend_412(ts, msg->getSD());
		return ;
	}
	nick = msg->getParams()[3];
	if (nick != tc->botname && msg->getParams()[1] == tc->channelBJ)
	{
		std::cout << "Adding " << nick << " to bj players." << std::endl;
		tc->bjPlayers.push_back(nick);
	}
}