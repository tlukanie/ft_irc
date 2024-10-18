/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CardDeck.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/10/18 13:24:23 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CardDeck.hpp"

CardDeck::CardDeck(void)
{
	this->_size = 52;
	for (int i = 0; i < 52; i++)
		this->_deck.push_back(i);
}

CardDeck::CardDeck(unsigned char size)
{
	this->_size = size;
	for (unsigned char i = 0; i < size; i++)
		this->_deck.push_back(i);
}

CardDeck::~CardDeck(void)
{
}

int	CardDeck::drawCard(void)
{
	int	pos;
	int	card;
	if (!this->_deck.size())
		return (-1);
	pos = rand() % this->_deck.size();
	card = this->_deck[pos];
	this->_deck.erase(this->_deck.begin() + pos);
	std::cout << "Deck size: " << this->_deck.size() << std::endl;
	return (card);
}

int	CardDeck::getDeckSize(void)
{
	return (this->_size);
}

void	CardDeck::refill(void)
{
	this->_deck.clear();
	for (unsigned char i = 0; i < this->_size; i++)
		this->_deck.push_back(i);
}
