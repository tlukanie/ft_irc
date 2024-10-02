/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CardPlayer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 15:46:10 by okraus           ###   ########.fr       */
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
		this->_cards.insert(card);
}

void	CardPlayer::emptyHand(void)
{
	this->_cards.clear();
	this->_stand = false;
}

std::set<int> const	&CardPlayer::showHand(void)
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
