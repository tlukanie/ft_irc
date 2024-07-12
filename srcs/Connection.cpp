/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/07/12 16:33:25 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(void)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Default constructor of the Connection class called.";
	ft_uncolorize();
	std::cout << std::endl;
	this->_sd = 0;
	this->_port = 0;
	this->_ip = "";
	this->_reading_flag = true;
	this->_data_overflow_flag = false;
	this->_strikecount = 0;
}

Connection::Connection(const Connection& copy)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Copy constructor of the Connection class called.";
	ft_uncolorize();
	std::cout << std::endl;
	*this = copy;
}

Connection &Connection::operator = (const Connection &src)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Assigment operator of the Connection class called.";
	ft_uncolorize();
	std::cout << std::endl;
	(void)src;
	return (*this);
}

Connection::~Connection(void)
{
	if (DEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Destructor of the Connection class called.";
		ft_uncolorize();
		std::cout << std::endl;
	}
}

Connection::Connection(unsigned short sd, unsigned short port, std::string ip)
{
	if (DEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Overriden constructor of the Connection class called.";
		ft_uncolorize();
		std::cout << std::endl;
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "SD: " << sd;
		std::cout << ", port: " << port;
		std::cout << ", ip: " << ip;
		ft_uncolorize();
		std::cout << std::endl;
	}
	this->_sd = sd;
	this->_port = port;
	this->_ip = ip;
	this->_reading_flag = true;
	this->_data_overflow_flag = false;
	this->_strikecount = 0;
}

unsigned short	Connection::getPort(void)
{
	return (this->_port);
}

std::string	Connection::getIP(void)
{
	return (this->_ip);
}

std::string	Connection::getNick(void)
{
	return (this->_nick);
}

void	Connection::setNick(std::string nick)
{
	this->_nick = nick;
}

bool	Connection::getOverflowFlag(void)
{
	return (this->_data_overflow_flag);
}

void	Connection::setOverflowFlag(void)
{
	this->_data_overflow_flag = true;
}

void	Connection::unsetOverflowFlag(void)
{
	this->_data_overflow_flag = false;
}


