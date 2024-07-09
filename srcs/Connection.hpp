/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/07/09 18:30:53 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONNECTION_HPP
# define CONNECTION_HPP

//# include <cstddef>
# include <iostream>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>

# define RECEIVING true
# define SENDING false

class Connection
{
	public:
		Connection(void);
		Connection(const Connection& copy);
		Connection &operator	= (const Connection &src);
		~Connection(void);
	
		Connection(unsigned short sd, unsigned short port, std::string ip);
		// getters and setters? for mode
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
		
		// buffer string (octet string? vector?)
		uint8_t	_buffer[512];
		// flag for invalid buffer (too much data without separator)
		bool	_buffer_overflow_flag;
		// strike count for number of invalid messages in a row???
		//int	strikecount;
		//nick
		//username
		//host?
		//realname
		//...
};

#endif