/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/09/15 10:32:34 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Default constructor of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;
	this->_channelName = "noname";
}

Channel::Channel(const Channel& copy)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Copy constructor of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;
	*this = copy;
}

Channel &Channel::operator = (const Channel &src)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Assigment operator of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;
	(void)src;
	return (*this);
}

Channel::~Channel(void)
{
	if (DEEPDEBUG)
	{
		ft_colorize(reinterpret_cast<uintptr_t>(this));
		std::cout << "Destructor of the Channel class called.";
		ft_uncolorize();
		std::cout << std::endl;
	}
}

Channel::Channel(std::string channelName, unsigned short sd)
{
	ft_colorize(reinterpret_cast<uintptr_t>(this));
	std::cout << "Overloaded constructor of the Channel class called.";
	ft_uncolorize();
	std::cout << std::endl;
	this->_channelName = channelName;
	this->_topic = "";
	this->_key = "";	//password
	this->_modeFlags = 0;
	this->_channelLimit = 0;
	this->_usersInChannel[sd] = true;
}

std::string	Channel::getChannelName(void)
{
	return (this->_channelName);
}

std::string	Channel::getTopic(void)
{
	return (this->_topic);
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

std::string	Channel::getKey(void)
{
	return (this->_key);
}

void	Channel::setKey(std::string key)
{
	this->_key = key;
}

unsigned int	Channel::getModeFlags(void)
{
	return (this->_modeFlags);
}

void	Channel::addModeFlags(unsigned int flag)
{
	this->_modeFlags |= flag;
}

void	Channel::removeModeFlags(unsigned int flag)
{
	this->_modeFlags &= ~flag;
}

int		Channel::getChannelLimit(void)
{
	return (this->_channelLimit);
}

void	Channel::setChannelLimit(int channelLimit)
{
	this->_channelLimit = channelLimit;
}

bool	Channel::isOperator(unsigned short sd)
{
	if (this->_usersInChannel.find(sd) != this->_usersInChannel.end())
		return (this->_usersInChannel[sd]);
	return (false); //should not happen?
}

void	Channel::addOperator(unsigned short sd)
{
	this->_usersInChannel[sd] = true;
}

void	Channel::removeOperator(unsigned short sd)
{
	this->_usersInChannel[sd] = false;
}

void	Channel::addUser(unsigned short sd)
{
	this->_usersInChannel[sd] = false;
}

int	Channel::removeUser(unsigned short sd)
{
	if (this->_usersInChannel.find(sd) != this->_usersInChannel.end())
		this->_usersInChannel.erase(sd);
	return (this->_usersInChannel.size());
}
