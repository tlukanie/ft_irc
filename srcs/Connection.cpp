/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/07/10 18:02:43 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(void)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Default constructor of the Connection class called.";
	ft_uncolorize();
	this->_sd = 0;
	this->_port = 0;
	this->_ip = "";
	this->_reading_flag = true;
	this->_data_overflow_flag = false;
	std::cout << std::endl;
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

}

unsigned short	Connection::getPort(void)
{
	return (this->_port);
}

std::string	Connection::getIP(void)
{
	return (this->_ip);
}


