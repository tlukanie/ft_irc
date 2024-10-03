/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_stand.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 10:24:32 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_stand(t_client *tc, std::string target, std::vector<std::string> params)
{
	std::string	reply;
	// start (list players playing)
	// stop

		//!hit
		//!stand
	(void)params;
	std::string	suits[4] = {"spades", "hearts", "diamonds", "clubs"};
	std::string	values[13] = {"ace", "two", "three", "four", "five", "six", "seven", "eight",
								"nine", "ten", "jack", "queen", "king"};

	if (!tc->bj.stand(target))
	{
		reply = tc->bj.announceWinner();
		tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
		tc->ready = true;
	}
	else
	{
		reply = "you are standing, waiting for other players";
		tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
		tc->ready = true;
	}
	// int	i = rand() % 52;
	// result = "Your card is: " + tc->cards[i] + "  " + values[i % 13] + " of " + suits[i / 13];
	
}