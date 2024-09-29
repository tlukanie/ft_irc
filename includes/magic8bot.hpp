/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic8bot.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 14:38:22 by okraus            #+#    #+#             */
/*   Updated: 2024/09/29 10:46:21 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef MAGIC8BOT_HPP
# define MAGIC8BOT_HPP
# include <stdio.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstring>
# include <csignal>
# include <stdlib.h> 
# include <errno.h> 
# include <unistd.h> //close 
# include <arpa/inet.h> //close 
# include <sys/types.h> 
# include <sys/socket.h> 
# include <netinet/in.h> 
# include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
# include <map>
# include <vector>
# include <algorithm>

# include "enums.hpp"
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

typedef struct s_client {
	std::string													botname;
	std::string													serverIP;
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
	std::multimap<int, Message*>								messages; //multimap of messages collected in one loop
	// std::map<std::string, void(*)(Message*, struct s_client*)>	commands; //map of commands and related functions
	t_debugger													debugger;
}	t_client;

// //COMMANDS
// void	irc_cap(Message* msg, struct s_server *ts);
// void	irc_pass(Message* msg, struct s_server *ts);
// void	irc_nick(Message* msg, struct s_server *ts);
// void	irc_user(Message* msg, struct s_server *ts);
// void	irc_ping(Message* msg, struct s_server *ts);
// void	irc_pong(Message* msg, struct s_server *ts);
// void	irc_quit(Message* msg, struct s_server *ts);
// /* CHANNEL OPERATIONS */
// void	irc_join(Message* msg, struct s_server *ts);
// void	irc_part(Message* msg, struct s_server *ts);
// void	irc_mode(Message* msg, struct s_server *ts);
// void	irc_topic(Message* msg, struct s_server *ts);
// void	irc_invite(Message* msg, struct s_server *ts);
// void	irc_kick(Message* msg, struct s_server *ts);
// void	irc_away(Message* msg, struct s_server *ts);
// void	irc_privmsg(Message* msg, struct s_server *ts);
// void	irc_notice(Message* msg, struct s_server *ts);
// void	irc_who(Message* msg, struct s_server *ts);
// void	irc_whois(Message* msg, struct s_server *ts);

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

int		irc_read_client_config(t_client *tc);
void	irc_init_debugger(s_debugger *debugger);

// int							ft_usage(void);
// int							ft_usage_port(void);
// int							ft_usage_debug(void);
// size_t						ok_crlf_finder(std::vector<uint8_t> data);
// // bool						isChannel(struct s_server *ts, std::string channelName);
// // bool						isNick(struct s_server *ts, std::string nick);
// // bool						isInChannel(struct s_server *ts, std::string channelName, std::string nick);
// // void						send_reply(struct s_server *ts, unsigned short sd, User *Sender, std::string text);
// // void						send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text);
// // int							remove_user_from_channel(struct s_server *ts, User *user, Channel *channel);
// // int							remove_user_from_server(struct s_server *ts, User *user, std::string reason);
// // int							add_user_to_channel(struct s_server *ts, User *user, Channel *channel);
// bool						isValidNick(std::string	nick);
// bool						isValidKey(std::string	key);
// std::vector<std::string>	ok_split(std::string str, char c);
// bool						ok_containsDuplicate(const std::string &text);
// std::string					getClient(struct s_server *ts, unsigned short sd);


// void	rpl_welcome_001(struct s_server *ts, unsigned short sd);
// void	rpl_endofwho_315(struct s_server *ts, std::string client, std::string mask);
// void	rpl_channelmodes_324(struct s_server *ts, unsigned short sd, Channel *channel);
// void	rpl_notopic_331(struct s_server *ts, unsigned short sd, std::string channelName);
// void	rpl_topic_332(struct s_server *ts, unsigned short sd, std::string channelName);
// void	rpl_inviting_341(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName);
// void	rpl_whoreply_352(struct s_server *ts, std::string client, std::string channelName, std::string nick);
// void	rpl_namreply_353(struct s_server *ts, unsigned short sd, std::string channelName);
// void	rpl_endofnames_366(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_unknownerror_400(struct s_server *ts, unsigned short sd, std::string command, std::string info);
// void	err_nosuchnick_401(struct s_server *ts, unsigned short sd, std::string nick);
// void	err_nosuchchannel_403(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_norecipient_411(struct s_server *ts, unsigned short sd, std::string command);
// void	err_notexttosend_412(struct s_server *ts, unsigned short sd);
// void	err_unknowncommand_421(struct s_server *ts, unsigned short sd, std::string command);
// void	err_nonicknamegiven_431(struct s_server *ts, unsigned short sd);
// void	err_erroneusnickname_432(struct s_server *ts, unsigned short sd, std::string nick);
// void	err_nicknameinuse_433(struct s_server *ts, unsigned short sd, std::string nick);
// void	err_usernotinchannel_441(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName);
// void	err_notonchannel_442(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_useronchannel_443(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName);
// void	err_needmoreparams_461(struct s_server *ts, unsigned short sd, std::string command);
// void	err_alreadyregistered_462(struct s_server *ts, unsigned short sd);
// void	err_passwdmismatch_464(struct s_server *ts, unsigned short sd);
// void	err_channelisfull_471(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_inviteonlychan_473(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_badchannelkey_475(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_badchanmask_476(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_chanopsprivsneeded_482(struct s_server *ts, unsigned short sd, std::string channelName);
// void	err_unknownmodeflag_501(struct s_server *ts, unsigned short sd);
// void	err_invalidmodeparam_696(struct s_server *ts, unsigned short sd, std::string target, std::string modeChar, std::string parameter, std::string description);


#endif
