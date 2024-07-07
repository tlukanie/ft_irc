/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colours.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:22 by okraus            #+#    #+#             */
/*   Updated: 2024/05/19 16:04:05 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "colours.hpp"

void	ft_colorize(uintptr_t i)
{
	//ESC[38:5:⟨n⟩m FOREGROUND
	//ESC[48:5:⟨n⟩m BACKGROUND
	i %= 997;
	i *= 599;
	i %= 216;
	i += 16;
	std::cout << "\033[1;";
	if ((i - 16) % 36 >= 18)
		std::cout << "38:5:" << 0 << ";";
	else
		std::cout << "38:5:" << 15 << ";";
	std::cout << "48:5:" << i << "m";

	//std::cout << i << " ";
}

std::string	ft_str_colorize(uintptr_t i)
{
	std::string	colour;
	std::string	num;
	i %= 997;
	i *= 599;
	i %= 216;
	i += 16;
	colour = "\033[1;";
	if ((i - 16) % 36 >= 18)
		colour += "38:5:0;";
	else
		colour += "38:5:15;";
	colour += "48:5:";
	if (i / 100)
		num += i / 100 + '0';
	if (i / 10)
		num += (i % 100) / 10 + '0';
	num += (i % 10) + '0';
	colour += num;
	colour += "m";

	//std::cout << i << " ";
	return (colour);
}

void	ft_uncolorize(void)
{
	std::cout << "\033[0m";
}