/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_stand.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/04 16:13:26 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_stand(t_client *tc, std::string target, std::vector<std::string> params)
{
	std::string	reply;
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