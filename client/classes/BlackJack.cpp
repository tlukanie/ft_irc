/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlackJack.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 16:12:18 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlackJack.hpp"

BlackJack::BlackJack(void)
{
	this->_deck.refill();
}

BlackJack::~BlackJack(void)
{
	for (std::map<std::string, CardPlayer*>::iterator it = this->_players.begin(); it != this->_players.end(); it++)
		delete it->second;
}

void	BlackJack::addPlayer(std::string const &nick)
{
	this->_players[nick] = new CardPlayer(nick);
}

void	BlackJack::drawCard(std::string const &nick)
{
	this->_players[nick]->drawCard(this->_deck);
}

void	BlackJack::stand(std::string const &nick)
{
	this->_players[nick]->stand();
}

std::string BlackJack::showHand(std::string const &nick)
{
	std::set<int>	playerHand = this->_players[nick]->showHand();
	std::string	deck[52] = {"🂡", "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮",
							"🂱", "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾",
							"🃁", "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎",
							"🃑", "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞"};
	std::string	hand;

	for (std::set<int>::iterator it = playerHand.begin(); it != playerHand.end(); it++)
	{
		if (*it >= 0 && *it < 52)
			hand += deck[*it];
	}
	//mayne add current score to the hand
	return (hand);
}

std::string	BlackJack::announceWinner(void)
{
	//if all players standing, calculate scores and announce winner, show hands
	std::string	winnerStatement;

	return (winnerStatement);
}

void	BlackJack::newGame(void)
{
	this->_deck.refill();
	for (std::map<std::string, CardPlayer*>::iterator it = this->_players.begin(); it != this->_players.end(); it++)
	{
		it->second->emptyHand();
		it->second->drawCard(this->_deck);
		it->second->drawCard(this->_deck);
	}
}
