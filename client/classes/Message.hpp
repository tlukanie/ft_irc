/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:52:42 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 09:48:01 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// uint8_t			_buffer[512];
// int 				size;

// loop 0
// waiting on any activity
// if activity -> go to loop 1
// loop 1
// server reads data from users
// extract messages from the data and save valid ones in multimap
// if valid message go to loop 2
// loop 2
// process messages in multimap (and empty it)
// and send response to users
// if all messages processed go to loop 0

//for accessing function pointer in map of commands
//https://www.geeksforgeeks.org/inserting-elements-in-stdmap-insert-emplace-and-operator/

#pragma once
#ifndef MESSAGE_HPP
# define MESSAGE_HPP

//# include <cstddef>
# include <iostream>
# include <vector>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>
// # include "../../includes/ircserv.hpp"


# define RECEIVING true
# define SENDING false

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

class Message
{
	public:
		~Message(void);
	
		Message(std::string msg);
		
		// getters and setters? for mode
		std::string const				&getMessage(void);
		std::string const				&getPrefix(void);
		std::string const				&getNick(void);
		std::string const				&getUser(void);
		std::string const				&getHost(void);
		std::string const				&getCommand(void);
		std::vector<std::string> const	&getParams(void);
		
		//methods
		//extract message
		//store in buffer
		
		// buffer string (octet string? vector?)
		// class InvalidMessageException : public std::exception
		// {
		// 	public:
		// 		const char*		what() const throw();
		// };

	private:
		Message(void);
		Message(const Message& copy);
		Message &operator	= (const Message &src);
		// ??? number of users?
		//static int	users;
		// socket descriptor
		// more stuff will come here
		//<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
		std::string					_msg;
		//prefix string
		// <nick>[!<user>@<host>]
		std::string					_prefix;
		std::string					_nick;
		std::string					_user;
		std::string					_host;
		//command string
		std::string					_command;
		//vector of params strings
		std::vector<std::string>	_params;
};

#endif