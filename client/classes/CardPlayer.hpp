/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CardPlayer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 15:44:58 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CARDPLAYER_HPP
# define CARDPLAYER_HPP

//# include <cstddef>
# include <iostream>
# include <set>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>

# include "../../includes/magic8bot.hpp"
# include "CardDeck.hpp"

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

class CardPlayer
{
	public:
		~CardPlayer(void);
		CardPlayer(std::string const &name);
		
		void				drawCard(CardDeck &deck);
		void				emptyHand(void);
		std::set<int> const	&showHand(void);
		std::string const	&getName(void);
		bool				isStanding(void);
		void				stand(void);

	private:
		CardPlayer(void);
		CardPlayer(const CardPlayer& copy);
		CardPlayer &operator	= (const CardPlayer &src);

		std::set<int>	_cards;
		std::string		_name;
		bool			_stand;
};

#endif