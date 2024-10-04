/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CardPlayer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/10/04 16:09:15 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CardPlayer.hpp"

CardPlayer::CardPlayer(std::string const &name)
{
	this->_name = name;
	this->_stand = false;
}

CardPlayer::~CardPlayer(void)
{
}

void	CardPlayer::drawCard(CardDeck &deck)
{
	int	card;

	card = deck.drawCard();
	if (card >= 0)
		this->_cards.push_back(card);
}

void	CardPlayer::emptyHand(void)
{
	this->_cards.clear();
	this->_stand = false;
}

static int	score(std::string hand)
{
	int	aces = 0;
	size_t	i = 0;
	int sum = 0;

	if (hand == "AA")
		return (22);
	while (i < hand.size())
	{
		if ('1' < hand[i] && hand[i] <= '9')
			sum += hand[i] - '0';
		else if (hand[i] == 'A')
			aces += 1;
		else if ('A' < hand[i] && hand[i] <= 'Z')
			sum += 10;
		i++;
	}
	while (aces)
	{
		if (sum > 11 - aces)
			sum++;
		else
			sum += 11;
		aces--;
	}
	if (sum > 21)
		return (-sum);
	return (sum);
}

int	CardPlayer::getHandScore(void)
{
	std::string	hand;
	for (size_t i = 0; i < this->_cards.size(); i++)
	{
		if (this->_cards[i] % 13 == 0)
			hand += 'A';
		else if (this->_cards[i] % 13 < 9)
			hand += ('1' + (this->_cards[i] % 13));
		else
			hand += 'T';
	}
	return (score(hand));
}

std::vector<int> const	&CardPlayer::showHand(void)
{
	return (this->_cards);
}

std::string const	&CardPlayer::getName(void)
{
	return(this->_name);
}

bool	CardPlayer::isStanding(void)
{
 return (this->_stand);
}

void	CardPlayer::stand(void)
{
 this->_stand = true;
}
