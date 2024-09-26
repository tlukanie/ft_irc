/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 14:38:22 by okraus            #+#    #+#             */
/*   Updated: 2024/09/26 09:48:32 by okraus           ###   ########.fr       */
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
# include "../srcs/classes/User.hpp"
# include "../srcs/classes/Channel.hpp"
# include "../srcs/classes/Message.hpp"

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
class Message;

typedef struct s_debugger {
	bool						date;
	bool						time;
	bool						utime;
	int							precision;
	bool						colour;
	bool						extra;
	int							fd;
	DebugLvl					debuglvl;
	std::string					log; //log to be printed;
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

void	irc_mode(Message* msg, struct s_server *ts);

# include <iostream>
# include <sstream>
# include <stdint.h>

# define ERROR_BLINKING_COLOUR "\033[1;5;38:5:226;48:5:160m"
# define ERROR_COLOUR "\033[1;38:5:226;48:5:160m"
# define REDBG_COLOUR "\033[1;38:5:231;48:5:160m"
# define GREENBG_COLOUR "\033[1;38:5:0;48:5:34m"
# define BLUEBG_COLOUR "\033[1;38:5:231;48:5:33m"
# define CYANBG_COLOUR "\033[1;38:5:0;48:5:51m"
# define MAGENTABG_COLOUR "\033[1;38:5:231;48:5:201m"
# define YELLOWBG_COLOUR "\033[1;38:5:0;48:5:226m"
# define ORANGEBG_COLOUR "\033[1;38:5:0;48:5:214m"
# define WHITEBG_COLOUR "\033[1;38:5:0;48:5:231m"
# define GREYBG_COLOUR "\033[1;38:5:0;48:5:246m"
# define BLACKBG_COLOUR "\033[1;38:5:231;48:5:0m"
# define RED_COLOUR "\033[1;38:5:160m"
# define GREEN_COLOUR "\033[1;38:5:34m"
# define BLUE_COLOUR "\033[1;38:5:33m"
# define CYAN_COLOUR "\033[1;38:5:51m"
# define MAGENTA_COLOUR "\033[1;38:5:201m"
# define YELLOW_COLOUR "\033[1;38:5:226m"
# define ORANGE_COLOUR "\033[1;38:5:214m"
# define WHITE_COLOUR "\033[1;38:5:231m"
# define GREY_COLOUR "\033[1;38:5:246m"
# define BLACK_COLOUR "\033[1;38:5:0m"
# define USERBG_COLOUR "\033[1;38:5:0;48:5:148m"
# define USER_COLOUR "\033[1;38:5:148m"
# define CHANNELBG_COLOUR "\033[1;38:5:0;48:5:147m"
# define CHANNEL_COLOUR "\033[1;38:5:147m"
# define NO_COLOUR "\033[0m"

void		ft_colorize(uintptr_t i);
std::string	ft_str_colorize(uintptr_t i);
void		ft_uncolorize(void);

# include <iostream>
# include <sstream>
# include <stdint.h>
# include <ctime>
# include <sys/time.h>
# include <iomanip>

# define ERROR_BLINKING_COLOUR "\033[1;5;38:5:226;48:5:160m"
# define ERROR_COLOUR "\033[1;38:5:226;48:5:160m"
# define REDBG_COLOUR "\033[1;38:5:231;48:5:160m"
# define GREENBG_COLOUR "\033[1;38:5:0;48:5:34m"
# define BLUEBG_COLOUR "\033[1;38:5:231;48:5:33m"
# define CYANBG_COLOUR "\033[1;38:5:0;48:5:51m"
# define MAGENTABG_COLOUR "\033[1;38:5:231;48:5:201m"
# define YELLOWBG_COLOUR "\033[1;38:5:0;48:5:226m"
# define ORANGEBG_COLOUR "\033[1;38:5:0;48:5:214m"
# define WHITEBG_COLOUR "\033[1;38:5:0;48:5:231m"
# define GREYBG_COLOUR "\033[1;38:5:0;48:5:246m"
# define BLACKBG_COLOUR "\033[1;38:5:231;48:5:0m"
# define RED_COLOUR "\033[1;38:5:160m"
# define GREEN_COLOUR "\033[1;38:5:34m"
# define BLUE_COLOUR "\033[1;38:5:33m"
# define CYAN_COLOUR "\033[1;38:5:51m"
# define MAGENTA_COLOUR "\033[1;38:5:201m"
# define YELLOW_COLOUR "\033[1;38:5:226m"
# define ORANGE_COLOUR "\033[1;38:5:214m"
# define WHITE_COLOUR "\033[1;38:5:231m"
# define GREY_COLOUR "\033[1;38:5:246m"
# define BLACK_COLOUR "\033[1;38:5:0m"
# define NO_COLOUR "\033[0m"

struct s_debugger;

//change int i to enum
void		ok_debugger(s_debugger *debugger, DebugLvl globalLevel, std::string message, std::string details, std::string extra);
std::string	ok_display_reply(s_debugger *debugger, std::string reply);
std::string	ok_display_buffer(s_debugger *debugger, std::string buff);
std::string	ok_display_real_buffer(s_debugger *debugger, std::vector<uint8_t> buff);
std::string	ok_display_real_buffer(bool colour, std::vector<uint8_t> buff);
std::string	ok_display_send_buffer(bool colour, std::vector<uint8_t> buff);
std::string	ok_display_message(s_debugger *debugger, std::string msg);


# include <iostream>
# include <sstream>


# define LOWER "abcdefghijklmnopqrstuvwxyz"
# define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define DIGITS "0123456789"
# define SPECIAL "[]{}^`|_\\"
# define KEY "!#$%&()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_abcdefghijklmnopqrstuvwxyz{|}~"

template <typename T> T	ok_strtoi(std::string str)
{
	std::stringstream	temp;
	T					num;

	temp << str;
	temp >> num;
	return (num);
}

template <typename T> std::string	ok_itostr(T num)
{
	std::stringstream	temp;
	std::string			str;

	temp << num;
	temp >> str;
	return (str);
}

int							ft_usage(void);
int							ft_usage_port(void);
int							ft_usage_debug(void);
size_t						ok_crlf_finder(std::vector<uint8_t> data);
bool						isChannel(struct s_server *ts, std::string channelName);
bool						isNick(struct s_server *ts, std::string nick);
bool						isInChannel(struct s_server *ts, std::string channelName, std::string nick);
void						send_reply(struct s_server *ts, unsigned short sd, User *Sender, std::string text);
void						send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text);
int							remove_user_from_channel(struct s_server *ts, User *user, Channel *channel);
int							remove_user_from_server(struct s_server *ts, User *user, std::string reason);
int							add_user_to_channel(struct s_server *ts, User *user, Channel *channel);
bool						isValidNick(std::string	nick);
bool						isValidKey(std::string	key);
std::vector<std::string>	ok_split(std::string str, char c);
bool						ok_containsDuplicate(const std::string &text);
std::string					getClient(struct s_server *ts, unsigned short sd);

void	ok_send_352(struct s_server *ts, std::string client, std::string channelName, std::string nick);
void	ok_send_315(struct s_server *ts, std::string client, std::string mask);
void	ok_send_421(struct s_server *ts, unsigned short sd, std::string command);

#endif
