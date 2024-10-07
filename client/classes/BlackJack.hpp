/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlackJack.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/10/07 16:25:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef BLACKJACK_HPP
# define BLACKJACK_HPP

//# include <cstddef>
# include <iostream>
# include <sstream>
# include <map>
# include <algorithm>
# include <vector>
# include <stdint.h>
# include <ctime>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>


# include "CardDeck.hpp"
# include "CardPlayer.hpp"

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

# define BJ_NOGAME 0
# define BJ_STARTING 1
# define BJ_INPROGRESS 2
# define BJ_ENDED 4

# define BJ_GAMELENGTH 60

class BlackJack
{
	public:
		~BlackJack(void);
		BlackJack(void);

		void			addPlayer(std::string const &nick);
		// remove player?
		void			drawCard(std::string const &nick);
		int				stand(std::string const &nick);
		bool			isStanding(std::string const &nick);
		std::string 	showHand(std::string const &nick);
		std::string 	showHandSorted(std::string const &nick);
		std::string		announceWinner(void);
		void			newGame(void);
		int				getStatus(void);
		std::time_t		getEndTime(void);

	private:
		BlackJack(const BlackJack& copy);
		BlackJack &operator	= (const BlackJack &src);

		CardDeck							_deck;
		std::map<std::string, CardPlayer*>	_players;
		int									_activePlayers;
		int									_status;
		std::time_t							_endtime;
};

# include "../../includes/templates.tpp"

#endif