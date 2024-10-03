/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_blackjack.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 10:08:33 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_blackjack(t_client *tc, std::string target, std::vector<std::string> params)
{
	std::string	reply;
	// start (list players playing)
	// stop

		//!hit
		//!stand

	std::string	suits[4] = {"spades", "hearts", "diamonds", "clubs"};
	std::string	values[13] = {"ace", "two", "three", "four", "five", "six", "seven", "eight",
								"nine", "ten", "jack", "queen", "king"};
	if (params.size() < 2)
	{
		tc->messageOut += "NOTICE " + target + " :Not enough players" CRLF;
		tc->ready = true;
		return ;
	}
	for (size_t i = 0; i < params.size(); i++)
	{
		std::string	nick = params[i];
		tc->bj.addPlayer(nick);
		reply = "new game of BlackJack started";
		tc->messageOut += "PRIVMSG " + nick + " :" + reply + CRLF;
		tc->messageOut += "NOTICE " + nick + " :" + tc->bj.showHand(nick) + CRLF;
	}
	
	// int	i = rand() % 52;
	// result = "Your card is: " + tc->cards[i] + "  " + values[i % 13] + " of " + suits[i / 13];
	reply = "the game of BlackJack started";
	tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
	tc->ready = true;
}