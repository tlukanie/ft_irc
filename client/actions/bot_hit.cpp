/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_hit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/07 16:39:43 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_hit(t_client *tc, std::string target, std::vector<std::string> params)
{
	std::string	reply;
	// need to check if player already standing or game not in progress
	(void)params;
	if (!(tc->bj.getStatus() & BJ_INPROGRESS))
	{
		reply = "no game in progress at the moment, type '!blackjack' to start a new game";
		tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
		tc->ready = true;
		return ;
	}
	if (tc->bj.isStanding(target))
	{
		reply = "you are already standing.";
		tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
		tc->ready = true;
		return ;
	}
	std::string	suits[4] = {"spades", "hearts", "diamonds", "clubs"};
	std::string	values[13] = {"ace", "two", "three", "four", "five", "six", "seven", "eight",
								"nine", "ten", "jack", "queen", "king"};
	tc->bj.drawCard(target);
	
	reply = "you draw a card";
	tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
	tc->messageOut += "NOTICE " + target + " :" + tc->bj.showHand(target) + CRLF;
	reply = target + " draws a card";
	for (std::vector<std::string>::iterator it = tc->bjPlayers.begin(); it != tc->bjPlayers.end(); it++)
	{
		if (*it != target)
			tc->messageOut += "NOTICE " + *it + " :" + reply + CRLF;
	}
	tc->messageOut += "NOTICE " + tc->channelBJ + " :" + reply + CRLF;
	tc->ready = true;
}