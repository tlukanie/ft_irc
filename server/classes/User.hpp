/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 13:04:01 by okraus           ###   ########.fr       */
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
# include "../../includes/ircserv.hpp"

# define RECEIVING true
# define SENDING false

# define PASSWORD 1
# define NICK 2
# define USER 4

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

class User
{
	public:
		~User(void);
	
		User(unsigned short sd, unsigned short port, std::string ip);
		
		// getters and setters? for mode
		unsigned short			getPort(void);
		unsigned short			getSD(void);
		std::string const		&getIP(void);
		std::string const		&getNick(void);
		std::string const		&getUserName(void);
		std::string const		&getHostName(void);
		std::string const		&getServerName(void);
		std::string const		&getRealName(void);
		std::string const		&getAwayMessage(void);
		bool					getOverflowFlag(void);
		unsigned int			getAuthFlags(void);
		bool					getAuthFlag(void);
		bool					getReadingFlag(void);
		
		void					setOverflowFlag(void);
		void					unsetOverflowFlag(void);
		void					addAuthFlag(unsigned int flag);
		// void					removeAuthFlag(unsigned int flag);
		void					setReadingFlag(void);
		void					unsetReadingFlag(void);
		bool					getFreedom(void);
		void					setFree(void);
		void					setNick(std::string const &nick);
		void					setUserName(std::string const &username);
		void					setHostName(std::string const &hostname);
		void					setServerName(std::string const &servername);
		void					setRealName(std::string const &realname);
		void					setAwayMessage(std::string const &awaymessage);
		std::string	const		print(bool colour);
		std::vector<uint8_t>	&getDataIn(void);
		std::vector<uint8_t>	&getDataOut(void);
		//methods
		//extract message
		//store in buffer
		

	private:
		User(void);
		User(const User& copy);
		User &operator	= (const User &src);
		// buffer string (octet string? vector?)
		std::vector<uint8_t>	_dataIn; //received data
		std::vector<uint8_t>	_dataOut; //data to send to client
		bool					_freedom; //set when user needs to be removed
		// ??? number of users?
		//static int	users;
		// socket descriptor
		unsigned short			_sd;
		// port
		unsigned short			_port;
		// ip address
		std::string				_ip;
		// reading or writing mode
		unsigned int			_authFlags; //valid password and unique nick and so on
		bool					_authFlag;
		bool					_reading_flag; //maybe useless
		// flag for invalid buffer (too much data without separator)
		bool					_data_overflow_flag;
		// strike count for number of invalid messages in a row???
		int						_strikecount;
		std::string				_nick;
		std::string				_username;
		std::string				_hostname;
		std::string				_servername;
		std::string				_realname;
		std::string				_awaymessage;
		// <username> <hostname> <servername> <realname>
		//nick	
		//username
		//host?
		//realname
		//...
};

#endif