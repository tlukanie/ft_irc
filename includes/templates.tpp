/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   templates.tpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:05:04 by okraus            #+#    #+#             */
/*   Updated: 2024/10/04 16:07:33 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef TEMPLATES_HPP
# define TEMPLATES_HPP


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

#endif
