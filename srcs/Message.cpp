/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:11:08 by okraus            #+#    #+#             */
/*   Updated: 2024/07/10 18:58:04 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(void)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Default constructor of the Message class called.";
	ft_uncolorize();
	this->_sd = 0;
	std::cout << std::endl;
}

Message::Message(const Message& copy)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Copy constructor of the Message class called.";
	ft_uncolorize();
	std::cout << std::endl;
	*this = copy;
}

Message &Message::operator = (const Message &src)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Assigment operator of the Message class called.";
	ft_uncolorize();
	std::cout << std::endl;
	(void)src;
	return (*this);
}

Message::~Message(void)
{
	if (DEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Destructor of the Message class called.";
		ft_uncolorize();
		std::cout << std::endl;
	}
}

// validate message during construction, if invalid throw exception

Message::Message(unsigned short sd, std::vector<uint8_t> data)
{
	if (DEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Overriden constructor of the Message class called.";
		ft_uncolorize();
		std::cout << std::endl;
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "SD: " << sd;
		std::cout << ", data: [";
		for (unsigned int i = 0; i < data.size(); i++)
			std::cout << data[i];
		std::cout << "]";
		ft_uncolorize();
		std::cout << std::endl;
	}
}

// unsigned short	Message::getPort(void)
// {
// 	return (this->_port);
// }

// std::string	Message::getIP(void)
// {
// 	return (this->_ip);
// }


