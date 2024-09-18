/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:07:48 by okraus            #+#    #+#             */
/*   Updated: 2024/09/18 18:14:57 by okraus           ###   ########.fr       */
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


//fixing for unicode in string
static int		ok_get_text_size(std::string text)
{
	int		size = 0;
	bool	skip = false;
	for (std::string::iterator it = text.begin(); it < text.end(); it++)
	{
		if (skip)
		{
			if (*it == 'm')
				skip = false;
			continue ;
		}
		if (*it == '\033')
		{
			skip = true;
			continue;
		}
		if (*it & 0b10000000)
		{
			if (*it & 0b01000000)
			{
				if (*it & 0b00100000)
				{
					if (*it & 0b00010000)
						it++;
					it++;
				}
				it++;
			}
		}
		++size;
	}
	return (size);
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
		int	text_size = ok_get_text_size(text);
		while (++text_size < 80)
			text += " ";
		text += extra;
	}
	output += level + text;
	if (debugger->colour)
		output += NO_COLOUR;
	std::cout << output << std::endl;
}

// std::string	ok_display_buffer(s_debugger *debugger, octets_t octet)
// {
	
// }

std::string	ok_display_reply(s_debugger *debugger, std::string reply)
{
	std::string	str;
	std::string	ascii_reply;

	for (std::string::iterator it = reply.begin(); it != reply.end(); it++)
	{
		if (std::isprint(*it))
			ascii_reply += *it;
		else if (*it == '\n')
			ascii_reply += "␤";
		else if (*it == '\r')
			ascii_reply += "␍";
		else
			ascii_reply += "�";
	}
	if (debugger->colour)
	{
		str = "[" ORANGE_COLOUR + ascii_reply + NO_COLOUR GREY_COLOUR "]";
	}
	else
	{
		str += "[" + ascii_reply + "]";
	}
	return (str);
}


std::string	ok_display_buffer(s_debugger *debugger, std::string buff)
{
	std::string	str;
	std::string	ascii_buff;

	for (std::string::iterator it = buff.begin(); it != buff.end(); it++)
	{
		if (std::isprint(*it))
			ascii_buff += *it;
		else if (*it == '\n')
			ascii_buff += "␤";
		else if (*it == '\r')
			ascii_buff += "␍";
		else
			ascii_buff += "�";
	}
	if (debugger->colour)
	{
		str = "[" CYAN_COLOUR + ascii_buff + NO_COLOUR GREY_COLOUR "]";
	}
	else
	{
		str += "[" + ascii_buff + "]";
	}
	return (str);
}

std::string	ok_display_real_buffer(s_debugger *debugger, std::vector<uint8_t> buff)
{
	std::string	str;
	std::string	ascii_buff;

	for (std::vector<uint8_t>::iterator it = buff.begin(); it != buff.end(); it++)
	{
		if (std::isprint(*it))
			ascii_buff += *it;
		else if (*it == '\n')
			ascii_buff += "␤";
		else if (*it == '\r')
			ascii_buff += "␍";
		else
			ascii_buff += "�";
	}
	if (debugger->colour)
	{
		str = "[" BLUE_COLOUR + ascii_buff + NO_COLOUR GREY_COLOUR "]";
	}
	else
	{
		str += "[" + ascii_buff + "]";
	}
	return (str);
}

std::string	ok_display_real_buffer(bool colour, std::vector<uint8_t> buff)
{
	std::string	str;
	std::string	ascii_buff;

	for (std::vector<uint8_t>::iterator it = buff.begin(); it != buff.end(); it++)
	{
		if (std::isprint(*it))
			ascii_buff += *it;
		else if (*it == '\n')
			ascii_buff += "␤";
		else if (*it == '\r')
			ascii_buff += "␍";
		else
			ascii_buff += "�";
	}
	if (colour)
	{
		str = "[" BLUE_COLOUR + ascii_buff + NO_COLOUR "]";
	}
	else
	{
		str += "[" + ascii_buff + "]";
	}
	return (str);
}

std::string	ok_display_send_buffer(bool colour, std::vector<uint8_t> buff)
{
	std::string	str;
	std::string	ascii_buff;

	for (std::vector<uint8_t>::iterator it = buff.begin(); it != buff.end(); it++)
	{
		if (std::isprint(*it))
			ascii_buff += *it;
		else if (*it == '\n')
			ascii_buff += "␤";
		else if (*it == '\r')
			ascii_buff += "␍";
		else
			ascii_buff += "�";
	}
	if (colour)
	{
		str = "[" YELLOW_COLOUR + ascii_buff + NO_COLOUR GREY_COLOUR "]";
	}
	else
	{
		str += "[" + ascii_buff + "]";
	}
	return (str);
}


std::string	ok_display_message(s_debugger *debugger, std::string msg)
{
	std::string	str;
	std::string	ascii_msg;

	for (std::string::iterator it = msg.begin(); it != msg.end(); it++)
	{
		if (std::isprint(*it))
			ascii_msg += *it;
		else if (*it == '\n')
			ascii_msg += "␤";
		else if (*it == '\r')
			ascii_msg += "␍";
		else
			ascii_msg += "�";
	}
	if (debugger->colour)
	{
		str = "[" RED_COLOUR + ascii_msg + NO_COLOUR GREY_COLOUR "]";
	}
	else
	{
		str += "[" + ascii_msg + "]";
	}
	return (str);
}