/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_time.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 10:50:34 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_time(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string			reply;
	std::time_t			time_now;
	std::stringstream	temp;
	std::string			time;

	time_now = std::time(NULL);
	std::tm tm = *(std::localtime(&time_now));
	temp
	<< std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
	<< std::setfill('0') << std::setw(2) << tm.tm_min << ":"
	<< std::setfill('0') << std::setw(2) << tm.tm_sec;
	temp >> time;

	reply = "Current server time is: " + time;
	tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
	tc->ready = true;
}