/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_date.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 10:46:29 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_date(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string			reply;
	std::stringstream	ss;
	std::time_t 		time_now;
	//  std::tm tm{}; // get_time does not set all fields hence {}
    // tm.tm_year = 2020 - 1900; // 2020
    // tm.tm_mon = 7 - 1; // July
    // tm.tm_mday = 15; // 15th
    // tm.tm_hour = 10;
    // tm.tm_min = 15;
	//std::tm tm;
	std::string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	std::string weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	time_now = std::time(NULL);
	std::string	ending;
	tm tm = *(std::localtime(&time_now));
	if (tm.tm_mday % 10 == 1)
		ending = "st";
	else if (tm.tm_mday % 10 == 2)
		ending = "nd";
	else if (tm.tm_mday % 10 == 3)
		ending = "rd";
	else
		ending = "th"; 
	ss << "Current date is "
	// << tm.tm_year + 1900 << "_"
	// << std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << "_"
	// << std::setfill('0') << std::setw(2) << tm.tm_mday << " "
	<< weekdays[tm.tm_wday] << ", "
	<< tm.tm_mday << ending << " of "
	<< months[tm.tm_mon] << " "
	<< tm.tm_year + 1900 << ".";
	// << "_"
	// << std::setfill('0') << std::setw(2) << tm.tm_hour
	// << std::setfill('0') << std::setw(2) << tm.tm_min
	// << std::setfill('0') << std::setw(2) << tm.tm_sec
	// << "] ";



	reply = ss.str(); 
	tc->messageOut += "NOTICE " + target + " :" + reply + CRLF;
	tc->ready = true;
}