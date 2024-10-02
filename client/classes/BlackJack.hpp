/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlackJack.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 16:11:41 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef BLACKJACK_HPP
# define BLACKJACK_HPP

//# include <cstddef>
# include <iostream>
# include <map>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>

# include "../../includes/magic8bot.hpp"
# include "CardDeck.hpp"
# include "CardPlayer.hpp"

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

class BlackJack
{
	public:
		~BlackJack(void);
		BlackJack(void);

		void			addPlayer(std::string const &nick);
		void			drawCard(std::string const &nick);
		void			stand(std::string const &nick);
		std::string 	showHand(std::string const &nick);
		std::string		announceWinner(void);
		void			newGame(void);

	private:
		BlackJack(const BlackJack& copy);
		BlackJack &operator	= (const BlackJack &src);

		CardDeck							_deck;
		std::map<std::string, CardPlayer*>	_players;
};

#endif