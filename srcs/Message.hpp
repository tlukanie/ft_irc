/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:52:42 by okraus            #+#    #+#             */
/*   Updated: 2024/07/12 10:53:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// uint8_t			_buffer[512];
// int 				size;

// loop 0
// waiting on any activity
// if activity -> go to loop 1
// loop 1
// server reads data from connections
// extract messages from the data and save valid ones in multimap
// if valid message go to loop 2
// loop 2
// process messages in multimap (and empty it)
// and send response to connections
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
# include "colours.hpp"


# define RECEIVING true
# define SENDING false

# ifndef DEBUG
#  define DEBUG 1
# endif

class Message
{
	public:
		Message(void);
		Message(const Message& copy);
		Message &operator	= (const Message &src);
		~Message(void);
	
		Message(unsigned short sd, std::string msg);
		
		// getters and setters? for mode
		std::string		getCommand(void);

		
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
		// ??? number of connections?
		//static int	connections;
		// socket descriptor
		unsigned short				_sd;
		// more stuff will come here
		//<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
		//prefix string
		std::string					_prefix;
		//command string
		std::string					_command;
		//vector of params strings
		std::vector<std::string>	_params;
};

#endif