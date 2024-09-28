/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:45:57 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 13:03:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
	// ft_colorize(reinterpret_cast<uintptr_t>(this));
	// std::cout << "Default constructor of the Channel class called.";
	// ft_uncolorize();
	// std::cout << std::endl;
	this->_channelName = "noname";
}

Channel::Channel(const Channel& copy)
{
	// ft_colorize(reinterpret_cast<uintptr_t>(this));
	// std::cout << "Copy constructor of the Channel class called.";
	// ft_uncolorize();
	// std::cout << std::endl;
	*this = copy;
}

Channel &Channel::operator = (const Channel &src)
{
	// ft_colorize(reinterpret_cast<uintptr_t>(this));
	// std::cout << "Assigment operator of the Channel class called.";
	// ft_uncolorize();
	// std::cout << std::endl;
	(void)src;
	return (*this);
}

Channel::~Channel(void)
{
	// if (DEEPDEBUG)
	// {
	// 	// ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	// std::cout << "Destructor of the Channel class called.";
	// 	// ft_uncolorize();
	// 	// std::cout << std::endl;
	// }
}

Channel::Channel(std::string channelName, unsigned short sd)
{
	// ft_colorize(reinterpret_cast<uintptr_t>(this));
	// std::cout << "Overloaded constructor of the Channel class called.";
	// ft_uncolorize();
	// std::cout << std::endl;
	this->_channelName = channelName;
	this->_topic = "";
	this->_key = "";	//password
	this->_modeFlags = 0;
	this->_channelLimit = 0;
	this->_usersInChannel[sd] = true;
}

std::string const	&Channel::getChannelName(void)
{
	return (this->_channelName);
}

std::string const	&Channel::getTopic(void)
{
	return (this->_topic);
}

void	Channel::setTopic(std::string const &topic)
{
	this->_topic = topic;
}

std::string const	&Channel::getKey(void)
{
	return (this->_key);
}

void	Channel::setKey(std::string const &key)
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

int		Channel::getUsers(void)
{
	return (this->_usersInChannel.size());
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

bool	Channel::hasUser(unsigned short sd)
{
	if (this->_usersInChannel.find(sd) != this->_usersInChannel.end())
		return (true);
	return (false);
}

void	Channel::addOperator(unsigned short sd)
{
	this->_usersInChannel[sd] = true;
}

void	Channel::removeOperator(unsigned short sd)
{
	this->_usersInChannel[sd] = false;
}

int	Channel::addUser(unsigned short sd)
{
	this->_usersInChannel[sd] = false;
	return (this->_usersInChannel.size());
}

int	Channel::removeUser(unsigned short sd)
{
	if (this->_usersInChannel.find(sd) != this->_usersInChannel.end())
		this->_usersInChannel.erase(sd);
	return (this->_usersInChannel.size());
}

// std::string						_channelName;
// std::string						_topic;
// std::string						_key;	//password
// unsigned int					_modeFlags;
// int								_channelLimit;
// std::map<unsigned short, bool>	_usersInChannel;	
// # define CHANNEL_INVITE	1	// +i
// # define CHANNEL_TOPIC	2	// +t
// # define CHANNEL_KEY	4	// +k
// # define CHANNEL_LIMIT	8	// +l
std::string const	Channel::print(bool colour)
{
	std::string	info;
	std::string	clr;
	std::string	clrbg;
	std::string	end;

	if (colour)
	{
		clr = CHANNEL_COLOUR;
		clrbg = CHANNELBG_COLOUR;
		end = NO_COLOUR;
	}
	info = "[" + clrbg + this->_channelName + end + "]\n";
	info += "Topic: " + clr + this->_topic + end + "\n";
	info += "Key: " + clr + this->_key + end + "\n";
	info += "Channel limit: " + clr + ok_itostr(this->_channelLimit) + end + "\n";
	info += "Invite only: " + clr + (this->_modeFlags & CHANNEL_INVITE ? "Y" : "N") + end + " | Topic by mods: " + clr + (this->_modeFlags & CHANNEL_TOPIC ? "Y" : "N") + end + " | Channel key: " + clr + (this->_modeFlags & CHANNEL_KEY ? "Y" : "N") + end + " | Channel limit: " + clr + (this->_modeFlags & CHANNEL_LIMIT ? "Y" : "N") + end + "\n";
	for (std::map<unsigned short, bool>::iterator it = this->_usersInChannel.begin(); it != this->_usersInChannel.end(); it++)
	{
		info += "Sd: " + clr + ok_itostr(it->first) + end + (it->second ? " is a channel operator.\n" : " is not a channel operator.\n");
	}
	return (info);
}
