/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:08:26 by okraus            #+#    #+#             */
/*   Updated: 2024/09/05 13:37:57 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <sstream>
# include "colours.hpp"

template <typename T> T	ok_strtoi(std::string str)
{
	std::stringstream	temp;
	T					num;

	temp << str;
	temp >> num;
	return (num);
}

template <typename T> std::string	ok_itostr(T num)
{
	std::stringstream	temp;
	std::string			str;

	temp << num;
	temp >> str;
	return (str);
}

int	ft_usage(void);
int	ft_usage_port(void);
int	ft_usage_debug(void);

#endif
