/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_stand.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/07 16:41:44 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_stand(t_client *tc, std::string target, std::vector<std::string> params)
{
	std::string	reply;
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
	(void)params;
	std::string	suits[4] = {"spades", "hearts", "diamonds", "clubs"};
	std::string	values[13] = {"ace", "two", "three", "four", "five", "six", "seven", "eight",
								"nine", "ten", "jack", "queen", "king"};

	if (!tc->bj.stand(target))
	{
		bj_end(tc);
	}
	else
	{
		reply = "you are standing, waiting for other players";
		tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
		reply = target + " stands.";
		for (std::vector<std::string>::iterator it = tc->bjPlayers.begin(); it != tc->bjPlayers.end(); it++)
		{
			if (*it != target)
				tc->messageOut += "NOTICE " + *it + " :" + reply + CRLF;
		}
		tc->messageOut += "NOTICE " + tc->channelBJ + " :" + reply + CRLF;
		tc->ready = true;
	}
	// int	i = rand() % 52;
	// result = "Your card is: " + tc->cards[i] + "  " + values[i % 13] + " of " + suits[i / 13];
}