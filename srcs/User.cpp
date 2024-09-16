/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/09/16 15:23:02 by okraus           ###   ########.fr       */
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
	this->_nick = nick;
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

bool	User::getAuthFlag(void)
{
	return (this->_auth_flag);
}

void	User::setAuthFlag(void)
{
	this->_auth_flag = true;
}

void	User::unsetAuthFlag(void)
{
	this->_auth_flag = false;
}

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

