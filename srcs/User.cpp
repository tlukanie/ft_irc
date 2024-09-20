/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/09/20 12:14:57 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Default constructor of the User class called.";
	ft_uncolorize();
	std::cout << std::endl;
	this->_sd = 0;
	this->_port = 0;
	this->_ip = "";
	this->_authFlag = false;
	this->_authFlags = 0;
	this->_reading_flag = true;
	this->_data_overflow_flag = false;
	this->_strikecount = 0;
}

User::User(const User& copy)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Copy constructor of the User class called.";
	ft_uncolorize();
	std::cout << std::endl;
	*this = copy;
}

User &User::operator = (const User &src)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Assigment operator of the User class called.";
	ft_uncolorize();
	std::cout << std::endl;
	(void)src;
	return (*this);
}

User::~User(void)
{
	if (DEEPDEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Destructor of the User class called.";
		ft_uncolorize();
		std::cout << std::endl;
	}
}

User::User(unsigned short sd, unsigned short port, std::string ip)
{
	if (DEEPDEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Overriden constructor of the User class called.";
		ft_uncolorize();
		std::cout << std::endl;
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "SD: " << sd;
		std::cout << ", port: " << port;
		std::cout << ", ip: " << ip;
		ft_uncolorize();
		std::cout << std::endl;
	}
	this->_freedom = false;
	this->_sd = sd;
	this->_port = port;
	this->_ip = ip;
	this->_authFlag = false;
	this->_authFlags = 0;
	this->_reading_flag = true;
	this->_data_overflow_flag = false;
	this->_strikecount = 0;
}

unsigned short	User::getPort(void)
{
	return (this->_port);
}

unsigned short	User::getSD(void)
{
	return (this->_sd);
}

std::string	User::getIP(void)
{
	return (this->_ip);
}

std::string	User::getNick(void)
{
	return (this->_nick);
}

void	User::setNick(std::string nick)
{
	std::cout << "Changing nick: " << this->_nick << " to " << nick << "." << std::endl;
	this->_nick = nick;
	std::cout << "Nick is now: " << this->_nick << "." << std::endl;
}

std::string	User::getUserName(void)
{
	return (this->_username);
}

void	User::setUserName(std::string username)
{
	this->_username = username;
}

std::string	User::getHostName(void)
{
	return (this->_hostname);
}

void	User::setHostName(std::string hostname)
{
	this->_hostname = hostname;
}

std::string	User::getServerName(void)
{
	return (this->_servername);
}

void	User::setServerName(std::string servername)
{
	this->_servername = servername;
}

std::string	User::getRealName(void)
{
	return (this->_realname);
}

void	User::setRealName(std::string realname)
{
	this->_realname = realname;
}

std::string	User::getAwayMessage(void)
{
	return (this->_awaymessage);
}

void	User::setAwayMessage(std::string awaymessage)
{
	this->_awaymessage = awaymessage;
}

bool	User::getOverflowFlag(void)
{
	return (this->_data_overflow_flag);
}

void	User::setOverflowFlag(void)
{
	this->_data_overflow_flag = true;
}

void	User::unsetOverflowFlag(void)
{
	this->_data_overflow_flag = false;
}

unsigned int	User::getAuthFlags(void)
{
	return (this->_authFlags);
}

bool	User::getAuthFlag(void)
{
	return (this->_authFlag);
}

void	User::addAuthFlag(unsigned int flag)
{
	this->_authFlags |= flag;
	if ((this->_authFlags & PASSWORD) && (this->_authFlags & NICK) && (this->_authFlags & USER))
		this->_authFlag = true;
}

// void	User::removeAuthFlag(unsigned int flag)
// {
// 	this->_authFlags &= ~flag;
// }

bool	User::getReadingFlag(void)
{
	return (this->_reading_flag);
}

void	User::setReadingFlag(void)
{
	this->_reading_flag = true;
}

void	User::unsetReadingFlag(void)
{
	this->_reading_flag = false;
}


// bool			_freedom; //set when user needs to be removed
// unsigned short	_sd;
// unsigned short	_port;
// std::string		_ip;
// bool			_auth_flag; 
// bool			_reading_flag;
// bool			_data_overflow_flag;
// int				_strikecount;
// std::string		_nick;
// std::string		_username;
// std::string		_hostname;
// std::string		_servername;
// std::string		_realname;
// std::string		_awaymessage;

std::string	User::print(bool colour)
{
	std::string	info;
	std::string	clr;
	std::string	clrbg;
	std::string	end;

	if (colour)
	{
		clr = USER_COLOUR;
		clrbg = USERBG_COLOUR;
		end = NO_COLOUR;
	}
	info = "[" + clrbg + this->_nick + end + "]\n";
	info += "sd: " + clr + ok_itostr(this->_sd) + end + " | IP: " + clr + this->_ip + end + " | Port: " + clr+ ok_itostr(this->_port) + end + "\n";
	info += "Authorized: " + clr + (this->_authFlag ? "Y" : "N") + end + " | Reading: " + clr + (this->_reading_flag ? "Y" : "N") + end + " | Data overflow: " + clr + (this->_data_overflow_flag ? "Y" : "N") + end + "\n";
	info += "Strike counter: " + clr + ok_itostr(this->_strikecount) + end + "\n";
	info += "Username: " + clr + this->_username + end + "\n";
	info += "Hostname: " + clr + this->_hostname + end + "\n";
	info += "Servername: " + clr + this->_servername + end + "\n";
	info += "Real name: " + clr + this->_realname + end + "\n";
	info += "Away message: " + clr + this->_awaymessage + end + "\n";
	info += "Buffer In : " + ok_display_real_buffer(colour, this->_data) + "\n";
	info += "Buffer Out: " + ok_display_real_buffer(colour, this->_data_out);
	return (info);
}
