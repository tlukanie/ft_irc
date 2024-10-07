/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bj_end.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:41:45 by okraus            #+#    #+#             */
/*   Updated: 2024/10/07 16:41:52 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bj_end(t_client *tc)
{
	std::string	reply;
	std::string	winnerAnnouncement = tc->bj.announceWinner();
	//show hands of all players to all
	for (std::vector<std::string>::iterator it = tc->bjPlayers.begin(); it != tc->bjPlayers.end(); it++)
	{
		for (std::vector<std::string>::iterator it2 = tc->bjPlayers.begin(); it2 != tc->bjPlayers.end(); it2++)
		{
			reply = *it2 + ": " + tc->bj.showHandSorted(*it2);
			tc->messageOut += "NOTICE " + *it + " :" + reply + CRLF;
		}
		tc->messageOut += "NOTICE " + *it + " :" + winnerAnnouncement + CRLF;
	}
	tc->messageOut += "NOTICE " + tc->channelBJ + " :" + winnerAnnouncement + CRLF;
	tc->ready = true;
}