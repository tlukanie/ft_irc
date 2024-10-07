/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_315.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:11:13 by tlukanie          #+#    #+#             */
/*   Updated: 2024/10/07 16:12:36 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there
//not sending messages to chanops only, yet...

void	irc_315(Message* msg, struct s_client *tc)
{
	(void)msg;
	std::string	reply;

	if (tc->bjPlayers.size() < 2)
	{
		tc->messageOut += "NOTICE " + tc->channelBJ + " :Not enough players" CRLF;
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