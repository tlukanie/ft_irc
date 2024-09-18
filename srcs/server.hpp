/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 14:38:22 by okraus            #+#    #+#             */
/*   Updated: 2024/09/18 17:25:30 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP
# include <stdio.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstring>
# include <csignal>
# include <string.h> //strlen 
# include <stdlib.h> 
# include <errno.h> 
# include <unistd.h> //close 
# include <arpa/inet.h> //close 
# include <sys/types.h> 
# include <sys/socket.h> 
# include <netinet/in.h> 
# include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
# include <map>
# include <algorithm>

# include "enums.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Message.hpp"
# include "debugger.hpp"
# include "utils.hpp"
# include "colours.hpp"

# define TRUE 1 
# define FALSE 0

# ifndef DEEPDEBUG
#  define DEEPDEBUG 1
# endif

# define MESSAGE "ECHO Daemon v1.0 \r\n"

# define MYENDL "\t\t" << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl
# define MYDEBUG std::string(__FILE__) + ":" + std::string(__func__) + ":" + ok_itostr(__LINE__)

# define READING_LOOP 1
# define SENDING_LOOP 2
# define BUFFER_SIZE 512
# define CRLF "\r\n"

class User;
class Channel;

typedef struct s_debugger {
	bool		date;
	bool		time;
	bool		utime;
	int			precision;
	bool		colour;
	bool		extra;
	int			fd;
	DebugLvl	debuglvl;
	std::string	log; //log to be printed;
}	t_debugger;

typedef struct s_server {
	std::string													servername;
	std::string													password;
	int															port; //unsigned short
	int															opt;
	int															addrlen;
	int															master_socket;
	int															new_socket;
	int															activity;
	int															valread;
	int															valsent;
	int															sd;
	int															max_sd;
	int															state;
	struct sockaddr_in											address;
	struct timeval												timeout;
	char														buffer[512]; //irc message is up to 512
	std::map<int, User*>										users; //map of Users/Users/Clients
	std::map<std::string, User*>								nicks;
	std::multimap<int, Message*>								messages; //multimap of messages collected in one loop
	std::map<std::string, void(*)(Message*, struct s_server*)>	commands; //map of commands and related functions
	//map nicks to sds?
	std::map<std::string, Channel*>								channels;
	std::multimap<std::string, Channel*>						user2channel;
	std::multimap<std::string, User*>							channel2user;
	t_debugger													debugger;
}	t_server;

#endif
