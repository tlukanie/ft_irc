/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/07/14 14:05:19 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONNECTION_HPP
# define CONNECTION_HPP

//# include <cstddef>
# include <iostream>
# include <vector>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>

# include "Message.hpp"
# include "colours.hpp"

# define RECEIVING true
# define SENDING false

# ifndef DEBUG
#  define DEBUG 1
# endif

class Connection
{
	public:
		Connection(void);
		Connection(const Connection& copy);
		Connection &operator	= (const Connection &src);
		~Connection(void);
	
		Connection(unsigned short sd, unsigned short port, std::string ip);
		
		// getters and setters? for mode
		unsigned short	getPort(void);
		std::string		getIP(void);
		std::string		getNick(void);
		bool			getOverflowFlag(void);
		bool			getReadingFlag(void);
		
		void			setOverflowFlag(void);
		void			unsetOverflowFlag(void);
		void			setReadingFlag(void);
		void			unsetReadingFlag(void);
		void			setNick(std::string nick);
		//methods
		//extract message
		//store in buffer
		
		// buffer string (octet string? vector?)

		std::vector<uint8_t>	_data;
	private:
		// ??? number of connections?
		//static int	connections;
		// socket descriptor
		unsigned short	_sd;
		// port
		unsigned short	_port;
		// ip address
		std::string	_ip;
		// reading or writing mode
		bool	_reading_flag;
		// flag for invalid buffer (too much data without separator)
		bool	_data_overflow_flag;
		// strike count for number of invalid messages in a row???
		int	_strikecount;
		std::string	_nick;
		//nick
		//username
		//host?
		//realname
		//...
};

#endif