/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_blackjack.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/04 15:07:22 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_blackjack(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string	reply;

	if (tc->bjPlayers.size() < 2)
	{
		tc->messageOut += "NOTICE " + target + " :Not enough players" CRLF;
		tc->ready = true;
		return ;
	}
	tc->bj.newGame();
	for (size_t i = 0; i < tc->bjPlayers.size(); i++)
	{
		std::string	nick = tc->bjPlayers[i];
		tc->bj.addPlayer(nick);
		reply = "new game of BlackJack started";
		tc->messageOut += "PRIVMSG " + nick + " :" + reply + CRLF;
		tc->messageOut += "NOTICE " + nick + " :" + tc->bj.showHand(nick) + CRLF;
	}
	
	// int	i = rand() % 52;
	// result = "Your card is: " + tc->cards[i] + "  " + values[i % 13] + " of " + suits[i / 13];
	reply = "the game of BlackJack started";
	tc->messageOut += "NOTICE " + tc->channelBJ + " :" + reply + CRLF;
	tc->ready = true;
}