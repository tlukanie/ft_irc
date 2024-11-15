/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CardDeck.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 09:54:18 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CARDDECK_HPP
# define CARDDECK_HPP

//# include <cstddef>
# include <iostream>
# include <vector>
# include <stdint.h>
# include <cstdlib> 
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>



# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

class CardDeck
{
	public:
		~CardDeck(void);
		CardDeck(void);
		CardDeck(unsigned char size);
		
		int		drawCard(void);
		int		getDeckSize(void);
		void	refill(void);

	private:
		CardDeck(const CardDeck& copy);
		CardDeck &operator	= (const CardDeck &src);

		unsigned char		_size;
		std::vector<int>	_deck;
		
};

#endif