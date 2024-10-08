/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlackJack.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/10/08 11:39:37 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlackJack.hpp"

BlackJack::BlackJack(void)
{
	this->_deck.refill();
	this->_activePlayers = 0;
	this->_status = BJ_NOGAME;
}

BlackJack::~BlackJack(void)
{
	for (std::map<std::string, CardPlayer*>::iterator it = this->_players.begin(); it != this->_players.end(); it++)
		delete it->second;
}

void	BlackJack::addPlayer(std::string const &nick)
{
	this->_players[nick] = new CardPlayer(nick);
	this->_players[nick]->drawCard(this->_deck);
	this->_players[nick]->drawCard(this->_deck);
	this->_activePlayers++;
}

void	BlackJack::drawCard(std::string const &nick)
{
	this->_players[nick]->drawCard(this->_deck);
}

int	BlackJack::stand(std::string const &nick)
{
	this->_players[nick]->stand();
	return (--(this->_activePlayers));
}

bool	BlackJack::isStanding(std::string const &nick)
{
	return (this->_players[nick]->isStanding());
}

static bool compareByValue(const int n1, const int n2)
{
	return (n1 % 13 < n2 % 13);
}

std::string BlackJack::showHandSorted(std::string const &nick)
{
	std::vector<int>	playerHand = this->_players[nick]->showHand();
	std::sort(playerHand.begin(), playerHand.end(), compareByValue);
	std::string	deck[52] = {"🂡", "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮",
							"🂱", "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾",
							"🃁", "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎",
							"🃑", "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞"};
	std::string	hand;

	for (std::vector<int>::iterator it = playerHand.begin(); it != playerHand.end(); it++)
	{
		if (*it >= 0 && *it < 52)
			hand += deck[*it] + " ";
	}
	//maybe add current score to the hand
	hand += "  " + ok_itostr(this->_players[nick]->getHandScore());
	return (hand);
}

std::string BlackJack::showHand(std::string const &nick)
{
	std::vector<int>	playerHand = this->_players[nick]->showHand();
	std::string	deck[52] = {"🂡", "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮",
							"🂱", "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾",
							"🃁", "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎",
							"🃑", "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞"};
	std::string	hand;

	for (std::vector<int>::iterator it = playerHand.begin(); it != playerHand.end(); it++)
	{
		if (*it >= 0 && *it < 52)
			hand += deck[*it] + " ";
	}
	//maybe add current score to the hand
	hand += "  " + ok_itostr(this->_players[nick]->getHandScore());
	return (hand);
}

std::string	BlackJack::announceWinner(void)
{
	std::string	winnerStatement;
	int			bestScore = -2147483648;
	
	//find the best score
	for (std::map<std::string, CardPlayer*>::iterator it = this->_players.begin(); it != this->_players.end(); it++)
	{
		if (it->second->getHandScore() > bestScore)
			bestScore = it->second->getHandScore();
	}
	//print names of those with it
	for (std::map<std::string, CardPlayer*>::iterator it = this->_players.begin(); it != this->_players.end(); it++)
	{
		if (it->second->getHandScore() == bestScore)
			winnerStatement += it->first + " ";
	}
	winnerStatement += "won the game of blackjack!";
	this->_status = BJ_NOGAME;
	return (winnerStatement);
}

void	BlackJack::newGame(void)
{
	this->_deck.refill();
	for (std::map<std::string, CardPlayer*>::iterator it = this->_players.begin(); it != this->_players.end(); it++)
	{
		delete it->second;
	}
	this->_players.clear();
	this->_activePlayers = 0;
	this->_endtime = std::time(NULL) + BJ_GAMELENGTH;
	this->_status = BJ_INPROGRESS;
	
}

int	BlackJack::getStatus(void)
{
	return (this->_status);
}

std::time_t	BlackJack::getEndTime(void)
{
	return (this->_endtime);
}
