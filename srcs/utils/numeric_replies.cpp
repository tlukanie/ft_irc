/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:22 by okraus            #+#    #+#             */
/*   Updated: 2024/09/26 14:13:34 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/ircserv.hpp"

//RPL_WELCOME (001)
void	tl_send_001(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "001 ";
	reply += client + " ";
	reply += ":Welcome to the ";
	reply += ts->servername;
	reply += " server, ";
	reply += ts->users[sd]->getNick() + "[!";
	reply += ts->users[sd]->getUserName() + "@";
	reply += ts->users[sd]->getHostName() + "]";
	send_reply(ts, sd, NULL, reply);
}

//RPL_ENDOFWHO (315)
void	ok_send_315(struct s_server *ts, std::string client, std::string mask)
{
	std::string	reply;
	reply = "315 ";
	reply += client + " ";
	reply += mask + " ";
	reply += ":End of WHO list";
	send_reply(ts, ts->nicks[client]->getSD(), NULL, reply);
}

//RPL_CHANNELMODEIS (324)
void	tl_send_324(struct s_server *ts, unsigned short sd, Channel *channel)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	// reply = "403 ";
	// reply += client + " ";
	// reply += channelName + " ";
	// reply += ":No such channel";
	reply = "324 ";
	reply += client + " ";
	reply += channel->getChannelName() + " ";
	if (channel->getModeFlags())
		reply += "+";
	if (channel->getModeFlags() & CHANNEL_INVITE)
		reply += "i";
	if (channel->getModeFlags() & CHANNEL_TOPIC)
		reply += "t";
	if (channel->getModeFlags() & CHANNEL_LIMIT)
		reply += "l";
	if (channel->getModeFlags() & CHANNEL_KEY)
		reply += "k";
	if (channel->getModeFlags() & CHANNEL_LIMIT)
		reply += " " + ok_itostr(channel->getChannelLimit());
	if (channel->getModeFlags() & CHANNEL_KEY)
		reply += " fake_key";
	send_reply(ts, sd, NULL, reply);

}

// "<client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
//RPL_WHOREPLY (352)
void	ok_send_352(struct s_server *ts, std::string client, std::string channelName, std::string nick)
{
	std::string	reply;
	std::string channel = channelName;
	reply = "352 ";
	reply += client + " ";
	if (!channel.size())
	{
		std::cout << "nick: " << nick << std::endl; 
		std::multimap<std::string, Channel*>::iterator it = ts->user2channel.find(nick);
		if (it != ts->user2channel.end())
			channel += it->second->getChannelName();
		else
			channel += "*";
		for (std::multimap<std::string, Channel*>::iterator iter = ts->user2channel.begin(); iter != ts->user2channel.end(); iter++)
			std::cout << "nick: " << it->first << " | chennel: " << it->second->getChannelName() << std::endl;
	}
	reply += channel + " ";
	reply += ts->nicks[nick]->getUserName() + " ";
	// if (ts->nicks[nick]->getIP()[0] == ':')
	// 	reply += "0";
	// reply += ts->nicks[nick]->getIP() + " ";
	reply += nick + " ";
	reply += ts->servername + " ";
	reply += nick + " ";
	reply += "H";
	if (channel != "*" && ts->channels[channel]->isOperator(ts->nicks[nick]->getSD()))
		reply += "@";
	reply += " ";
	reply += ":1 " + ts->nicks[nick]->getRealName();
	send_reply(ts, ts->nicks[client]->getSD(), NULL, reply);
}

//ERR_UNKNOWNERROR (400), Modern Documentation
void	tl_send_400(struct s_server *ts, unsigned short sd, std::string command, std::string info)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "400 ";
	reply += client + " ";
	reply += command + " ";
	reply += ":" + info;
	send_reply(ts, sd, NULL, reply);
}

//ERR_NOSUCHCHANNEL (403)
void	tl_send_403(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "403 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":No such channel";
	send_reply(ts, sd, NULL, reply);
}


//ERR_UNKNOWNCOMMAND (421)
void	ok_send_421(struct s_server *ts, unsigned short sd, std::string command)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "421 ";
	reply += client + " ";
	reply += command + " ";
	reply += ":Unknown command";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NONICKNAMEGIVEN (431)
void	ok_send_431(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "431 ";
	reply += client + " ";
	reply += ":No nickname given";
	send_reply(ts, sd, NULL, reply);
}

//ERR_ERRONEUSNICKNAME (432)
void	ok_send_432(struct s_server *ts, unsigned short sd, std::string nick)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "432 ";
	reply += client + " ";
	reply += nick + " ";
	reply += ":Erroneus nickname";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NICKNAMEINUSE (433)
void	ok_send_433(struct s_server *ts, unsigned short sd, std::string nick)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "433 ";
	reply += client + " ";
	reply += nick + " ";
	reply += ":Nickname is already in use";
	send_reply(ts, sd, NULL, reply);
}

//ERR_USERNOTINCHANNEL (441)
void	tl_send_441(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "441 ";
	reply += client + " ";
	reply += nick + " ";
	reply += channelName + " ";
	reply += ":They are not on that channel";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NOTONCHANNEL (442)
void	tl_send_442(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "442 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":You are not on that channel";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NEEDMOREPARAMS (461)
void	tl_send_461(struct s_server *ts, unsigned short sd, std::string command)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "461 ";
	reply += client + " ";
	reply += command + " ";
	reply += ":Not enough parameters";
	send_reply(ts, sd, NULL, reply);
}

//ERR_ALREADYREGISTERED (462)
void	tl_send_462(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "462 ";
	reply += client + " ";
	reply += ":You may not reregister";
	send_reply(ts, sd, NULL, reply);
}

//ERR_CHANOPRIVSNEEDED (482)
void	tl_send_482(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "482 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":You are not channel operator";
	send_reply(ts, sd, NULL, reply);
}

//ERR_UMODEUNKNOWNFLAG (501)
void	tl_send_501(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "501 ";
	reply += client + " ";
	reply += ":Unknown MODE flag";
	send_reply(ts, sd, NULL, reply);
}

//ERR_INVALIDMODEPARAM (696)
void	ok_send_696(struct s_server *ts, unsigned short sd, std::string target, std::string modeChar, std::string parameter, std::string description)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "696 ";
	reply += client + " ";
	reply += target + " ";
	reply += modeChar + " ";
	reply += parameter + " ";
	reply += ":" + description;
	send_reply(ts, sd, NULL, reply);
}