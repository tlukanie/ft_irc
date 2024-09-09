/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/09/09 14:48:35 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef USER_HPP
# define USER_HPP

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

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

class User
{
	public:
		User(void);
		User(const User& copy);
		User &operator	= (const User &src);
		~User(void);
	
		User(unsigned short sd, unsigned short port, std::string ip);
		
		// getters and setters? for mode
		unsigned short	getPort(void);
		std::string		getIP(void);
		std::string		getNick(void);
		std::string		getUserName(void);
		std::string		getHostName(void);
		std::string		getServerName(void);
		std::string		getRealName(void);
		bool			getOverflowFlag(void);
		bool			getAuthFlag(void);
		bool			getReadingFlag(void);
		
		void			setOverflowFlag(void);
		void			unsetOverflowFlag(void);
		void			setAuthFlag(void);
		void			unsetAuthFlag(void);
		void			setReadingFlag(void);
		void			unsetReadingFlag(void);
		void			setNick(std::string nick);
		void			setUserName(std::string username);
		void			setHostName(std::string hostname);
		void			setServerName(std::string servername);
		void			setRealName(std::string realname);
		//methods
		//extract message
		//store in buffer
		
		// buffer string (octet string? vector?)

		std::vector<uint8_t>	_data;
	private:
		// ??? number of users?
		//static int	users;
		// socket descriptor
		unsigned short	_sd;
		// port
		unsigned short	_port;
		// ip address
		std::string		_ip;
		// reading or writing mode
		bool			_auth_flag; //valid password and unique nick and so on
		bool			_reading_flag;
		// flag for invalid buffer (too much data without separator)
		bool			_data_overflow_flag;
		// strike count for number of invalid messages in a row???
		int				_strikecount;
		std::string		_nick;
		std::string		_username;
		std::string		_hostname;
		std::string		_servername;
		std::string		_realname;
		// <username> <hostname> <servername> <realname>
		//nick	
		//username
		//host?
		//realname
		//...
};

#endif