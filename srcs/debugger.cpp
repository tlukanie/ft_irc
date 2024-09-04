/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:07:48 by okraus            #+#    #+#             */
/*   Updated: 2024/09/01 15:04:30 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debugger.hpp"

void	ok_debugger(s_server *ts, DebugLvl debugLevel, std::string message, std::string details, std::string extra)
{
	std::string	output;
	std::string	colour;
	std::string	text;

	//for performance maybe move permanently in ts structure
	std::string		str[6] = {
		GREYBG_COLOUR "DDEBUG: " NO_COLOUR " " GREY_COLOUR,
		GREYBG_COLOUR "DEBUG:  " NO_COLOUR " " GREY_COLOUR,
		GREENBG_COLOUR "INFO:   " NO_COLOUR " " GREEN_COLOUR,
		YELLOWBG_COLOUR "NOTICE: " NO_COLOUR " " YELLOW_COLOUR,
		ORANGEBG_COLOUR "WARNING:" NO_COLOUR " " ORANGE_COLOUR,
		ERROR_COLOUR "ERROR:  " NO_COLOUR " " RED_COLOUR};

	// not printing if the global debug level is set for higher priority messages
	if (ts->debuglvl > debugLevel)
		return ;
	//assign appropriate string and colours
	colour += str[debugLevel];
	text += message;
	if (details.size())
		text += " " + details;
	// formatting and extra information
	if (ts->debuglvl == EXTRADEBUG)
	{
		text += " ";
		while (text.size() < 42)
			text += " ";
		text += extra;
	}
	text += NO_COLOUR;
	output = colour + text;
	std::cout << output << std::endl;
}