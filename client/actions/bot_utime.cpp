/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_utime.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 10:55:16 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_utime(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string			reply;
	std::time_t			time_now;
	std::stringstream	temp;
	std::string			utime;
	struct timeval		tv;

	time_now = std::time(NULL);
	gettimeofday(&tv, NULL);
	std::tm tm = *(std::localtime(&time_now));
	temp
	<< std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
	<< std::setfill('0') << std::setw(2) << tm.tm_min << ":"
	<< std::setfill('0') << std::setw(2) << tm.tm_sec << "."
	<< std::setfill('0') << std::setw(6) << tv.tv_usec;
	temp >> utime;
	reply = "Current server time is: " + utime;
	tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
	tc->ready = true;
}