/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_read_client_config.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 10:24:07 by okraus            #+#    #+#             */
/*   Updated: 2024/09/29 14:43:33 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"


// PORT=5555;
// #IP=10.12.1.3
// PASSWORD=abc;
// DEBUG_LVL=debug;			#debug, info, notice, warning, error, disabled
// DEBUG_FD=2; 
// #DEBUGFILE="debug.txt"
// DEBUG_DATE=0;			# show date
// DEBUG_TIME=1;			# show time
// DEBUG_UTIME=1;			# show microseconds
// DEBUG_UPRECISION=4;		# precision of time in microseconds
// DEBUG_COLOUR=1;			# use ansi escape codes
// DEBUG_EXTRA=1;			# show file:function:line


int	irc_read_client_config(t_client *tc)
{
	std::ifstream	file(".ft_ircbot.conf");
	std::string		line;
	std::string		key;
	std::string		value;

	tc->botname = "magic8bot";
	if (file.fail() || !file.is_open())
		return (1);
	while (std::getline(file, line))
	{
		size_t index = line.find('=');
		if (index == std::string::npos || index == line.size())
			continue ;
		key = line.substr(0, index);
		size_t end = line.find(';');
		if (line.size() && line[0] == '#')
			continue ;
		if (end == std::string::npos)
		{
			continue ;
		}
		value = line.substr(index + 1, end - index -1); //+1 to skip '=' and -1 to ignore ';'
		if (key == "PORT")
		{
			if (ok_strtoi<int>(value) <= 0 || ok_strtoi<int>(value) > 65535)
				return (1);
			tc->port = ok_strtoi<int>(value);
		}
		else if (key == "IP")
		{
			tc->serverIP = value;
		}
		else if (key == "BOTNAME")
		{
			tc->botname = value;
		}
		else if (key == "PASSWORD")
		{
			tc->password = value;
		}
		else if (key == "DEBUG_LVL")
		{
			if (value == "debug")
				tc->debugger.debuglvl = DEBUG;
			else if (value == "info")
				tc->debugger.debuglvl = INFO;
			else if (value == "notice")
				tc->debugger.debuglvl = NOTICE;
			else if (value == "warning")
				tc->debugger.debuglvl = WARNING;
			else if (value == "error")
				tc->debugger.debuglvl = ERROR;
			else if (value == "disabled")
				tc->debugger.debuglvl = DISABLED;
			else
				std::cerr << "Invalid DEBUG_LVL: " << value << std::endl;
		}
		else if (key == "DEBUG_FD")
		{
			if (value == "1")
				tc->debugger.fd = 1;
			else if (value == "2")
				tc->debugger.fd = 2;
			else
				std::cerr << "Invalid DEBUG_FD: " << value << std::endl;
		}
		else if (key == "DEBUG_DATE")
		{
			if (value == "0")
				tc->debugger.date = false;
			else if (value == "1")
				tc->debugger.date = true;
			else
				std::cerr << "Invalid DEBUG_DATE: " << value << std::endl;
		}
		else if (key == "DEBUG_TIME")
		{
			if (value == "0")
				tc->debugger.time = false;
			else if (value == "1")
				tc->debugger.time = true;
			else
				std::cerr << "Invalid DEBUG_TIME: " << value << std::endl;
		}
		else if (key == "DEBUG_UTIME")
		{
			if (value == "0")
				tc->debugger.utime = false;
			else if (value == "1")
				tc->debugger.utime = true;
			else
				std::cerr << "Invalid DEBUG_UTIME: " << value << std::endl;
		}
		else if (key == "DEBUG_UPRECISION")
		{
			if (value == "0")
				tc->debugger.precision = 0;
			else if (value == "1")
				tc->debugger.precision = 1;
			else if (value == "2")
				tc->debugger.precision = 2;
			else if (value == "3")
				tc->debugger.precision = 3;
			else if (value == "4")
				tc->debugger.precision = 4;
			else if (value == "5")
				tc->debugger.precision = 5;
			else if (value == "6")
				tc->debugger.precision = 6;
			else
				std::cerr << "Invalid DEBUG_UPRECISION: " << value << std::endl;
		}
		else if (key == "DEBUG_COLOUR")
		{
			if (value == "0")
				tc->debugger.colour = false;
			else if (value == "1")
				tc->debugger.colour = true;
			else
				std::cerr << "Invalid DEBUG_COLOUR: " << value << std::endl;
		}
		else if (key == "DEBUG_EXTRA")
		{
			if (value == "0")
				tc->debugger.extra = false;
			else if (value == "1")
				tc->debugger.extra = true;
			else
				std::cerr << "Invalid DEBUG_EXTRA: " << value << std::endl;
		}
		else
				std::cerr << "Unknown key: " << key << std::endl;
	}
	if (!tc->port)
		return (1);
	if (tc->password.size() == 0)
		return (1);
	return (0);
}