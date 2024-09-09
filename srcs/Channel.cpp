/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/09/09 14:40:31 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Default constructor of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;

}

Channel::Channel(const Channel& copy)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Copy constructor of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;
	*this = copy;
}

Channel &Channel::operator = (const Channel &src)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Assigment operator of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;
	(void)src;
	return (*this);
}

Channel::~Channel(void)
{
	if (DEEPDEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Destructor of the Channel class called.";
		ft_uncolorize();
		std::cout << std::endl;
	}
}