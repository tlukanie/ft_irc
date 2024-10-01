/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:11:08 by okraus            #+#    #+#             */
/*   Updated: 2024/10/01 09:24:38 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::~Message(void)
{
	// if (DEEPDEBUG)
	// {
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << "Destructor of the Message class called.";
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
	// }
}

// validate message during construction, if invalid throw exception

Message::Message(std::string msg)
{
	// std::string prefix;
	// std::string command;
	std::string param;
	// if (DEEPDEBUG)
	// {
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << "Overriden constructor of the Message class called.";
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << "SD: " << sd;
	// 	std::cout << ", msg: [";
	// 	std::cout << msg;
	// 	std::cout << "]";
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
	// }
	this->_msg = msg;
	//this->
	// if (msg.size() < 10)
	// 	throw std::runtime_error("Message too short");
	//std::string					_prefix;
	// //command string
	// std::string					_command;
	// //vector of params strings
	// std::vector<std::string>	_params;

	//https://www.rfc-editor.org/rfc/rfc1459#section-2.3
	//add checks on valid characters
	if (msg.size() && msg[0] == ':')
	{
		this->_prefix.assign(msg, 1, msg.find(' ') - 1);
		while (msg.size() && msg[0] != ' ')
			msg.erase(0, 1);
		while (msg.size() && msg[0] == ' ')
			msg.erase(0, 1);
	}
	// std::cout << "passed the prefix" << std::endl;
	if (msg.size() > 1)
	{
		this->_command.assign(msg, 0, msg.find(' '));
			while (msg.size() && msg[0] != ' ')
			msg.erase(0, 1);
		while (msg.size() && msg[0] == ' ')
			msg.erase(0, 1);
	}
	else
		throw std::runtime_error("Command missing");
	// std::cout << "passed the command" << std::endl;
	while (msg.size())
	{
		if (msg[0] == ':')
		{
			param.assign(msg, 1, std::string::npos);
			this->_params.push_back(param);
			break ;
		}
		param.assign(msg, 0, msg.find(' '));
		while (msg.size() && msg[0] != ' ')
			msg.erase(0, 1);
		while (msg.size() && msg[0] == ' ')
			msg.erase(0, 1);
		this->_params.push_back(param);
		param.clear();
		// std::cout << "processed param" << std::endl;
	}
	// std::cout << "passed the params" << std::endl;
	// if (DEEPDEBUG)
	// {
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << "PREFIX:";
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << this->_prefix;
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
		
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << "COMMAND:";
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
	// 	ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 	std::cout << this->_command;
	// 	ft_uncolorize();
	// 	std::cout << std::endl;
		
	// 	int i = 0;
	// 	for (std::vector<std::string>::iterator it = this->_params.begin(); it != this->_params.end(); it++)
	// 	{
	// 		ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 		std::cout << "PARAM: " << i;
	// 		ft_uncolorize();
	// 		std::cout << std::endl;
	// 		ft_colorize(reinterpret_cast<uintptr_t>(this));
	// 		std::cout << *it;
	// 		ft_uncolorize();
	// 		std::cout << std::endl;
	// 		i++;
	// 	}
	// }
}

std::string const	&Message::getMessage(void)
{
	return (this->_msg);
}

std::string const	&Message::getCommand(void)
{
	return (this->_command);
}

std::vector<std::string> const &Message::getParams(void)
{
	return (this->_params);
}

// more specific exceptions are needed