/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:07:48 by okraus            #+#    #+#             */
/*   Updated: 2024/09/05 14:46:13 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debugger.hpp"

static std::string	get_date(void)
{
	std::time_t			time_now;
	std::stringstream	temp;
	std::string			date;

	time_now = std::time(NULL);
	std::tm tm = *(std::localtime(&time_now));
	temp << tm.tm_year + 1900 << "_"
	<< std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << "_"
	<< std::setfill('0') << std::setw(2) << tm.tm_mday;
	temp >> date;
	return (date);
}

static std::string	get_time(void)
{
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
	return (time);
}

//gettimeofday is not allowed by the subject but helps with the logging
static std::string	get_utime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::stringstream	temp;
	std::string			utime;

	temp
	<< std::setfill('0') << std::setw(6) << tv.tv_usec;
	temp >> utime;
	return (utime);
}

void	ok_debugger(s_debugger *debugger, DebugLvl debugLevel, std::string message, std::string details, std::string extra)
{
	std::string	output;
	// std::string	colour;
	std::string	level;
	std::string	text;
	std::string	timestamp;

	//for performance maybe move permanently in ts structure
	std::string	bgcolours[6] = {
		GREYBG_COLOUR,
		GREYBG_COLOUR,
		GREENBG_COLOUR,
		YELLOWBG_COLOUR,
		ORANGEBG_COLOUR,
		ERROR_COLOUR};
	std::string	colours[6] = {
		GREY_COLOUR,
		GREY_COLOUR,
		GREEN_COLOUR,
		YELLOW_COLOUR,
		ORANGE_COLOUR,
		RED_COLOUR};
	std::string	levels[6] = {
		"DDEBUG: ",
		"DEBUG:  ",
		"INFO:   ",
		"NOTICE: ",
		"WARNING:",
		"ERROR:  "};

	// not printing if the global debug level is set for higher priority messages
	if (debugger->debuglvl > debugLevel)
		return ;
	// typedef struct s_debugger {
// 	bool		date;
// 	bool		time;
// 	bool		utime;
// 	bool		colour;
// 	bool		extra;
// 	int			fd;
// 	DebugLvl	debuglvl;
// }	t_debugger;
	if (debugger->date || debugger->time || debugger->utime)
	{
		if (debugger->colour)
			timestamp = colours[debugLevel];
		timestamp += "[";
		if (debugger->date)
		{
			timestamp += get_date();
			if (debugger->time || debugger->utime)
				timestamp += " ";
		}
		if (debugger->time)
		{
			timestamp += get_time();
			if (debugger->utime)
				timestamp += ".";
		}
		if (debugger->utime)
		{
			if (debugger->precision >= 0 && debugger->precision <= 6)
				timestamp += get_utime().substr(0, debugger->precision);
		}
		timestamp += "] ";
		if (debugger->colour)
			timestamp += NO_COLOUR;
	}
	output += timestamp;
	//assign appropriate string and colours
	if (debugger->colour)
		level = bgcolours[debugLevel] + levels[debugLevel] + NO_COLOUR " " + colours[debugLevel];
	else
		level = levels[debugLevel] + " ";
	text += message;
	if (details.size())
		text += " " + details;
	// formatting and extra information
	if (debugger->extra)
	{
		text += " ";
		while (text.size() < 42)
			text += " ";
		text += extra;
	}
	output += level + text;
	if (debugger->colour)
		output += NO_COLOUR;
	std::cout << output << std::endl;
}