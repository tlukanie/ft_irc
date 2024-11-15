/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:22 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 15:36:36 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/ircserv.hpp"

//RPL_WELCOME (001)
void	rpl_welcome_001(struct s_server *ts, unsigned short sd)
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
void	rpl_endofwho_315(struct s_server *ts, std::string client, std::string mask)
{
	std::string	reply;
	reply = "315 ";
	reply += client + " ";
	reply += mask + " ";
	reply += ":End of WHO list";
	send_reply(ts, ts->nicks[client]->getSD(), NULL, reply);
}

//RPL_CHANNELMODEIS (324)
void	rpl_channelmodes_324(struct s_server *ts, unsigned short sd, Channel *channel)
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

//RPL_NOTOPIC (331)
void	rpl_notopic_331(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "331 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":No topic is set";
	send_reply(ts, sd, NULL, reply);
}

//RPL_TOPIC (332)
void	rpl_topic_332(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "332 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":" + ts->channels[channelName]->getTopic();
	send_reply(ts, sd, NULL, reply);
}

//RPL_INVITING (341)
void	rpl_inviting_341(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "341 ";
	reply += client + " ";
	reply += nick + " ";
	reply += channelName;
	send_reply(ts, sd, NULL, reply);
}

// "<client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
//RPL_WHOREPLY (352)
void	rpl_whoreply_352(struct s_server *ts, std::string client, std::string channelName, std::string nick)
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

//RPL_NAMREPLY (353)
void	rpl_namreply_353(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "353 ";
	reply += client + " ";
	reply += "= ";
	reply += channelName + " ";
	reply += ":";
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		if (ts->channels[channelName]->isOperator(it->second->getSD()))
			reply += "@";
		reply += it->second->getNick() + " ";
	}
	reply.resize(reply.size() - 1);
	send_reply(ts, sd, NULL, reply);
}

//RPL_ENDOFNAMES (366)
void	rpl_endofnames_366(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "366 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":End of /NAMES list";
	send_reply(ts, sd, NULL, reply);
}

//ERR_UNKNOWNERROR (400), Modern Documentation
void	err_unknownerror_400(struct s_server *ts, unsigned short sd, std::string command, std::string info)
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

//ERR_NOSUCHNICK (401) 
void	err_nosuchnick_401(struct s_server *ts, unsigned short sd, std::string nick)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "401 ";
	reply += client + " ";
	reply += nick + " ";
	reply += ":No such nickname";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NOSUCHCHANNEL (403)
void	err_nosuchchannel_403(struct s_server *ts, unsigned short sd, std::string channelName)
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

//ERR_NORECIPIENT (411)
void	err_norecipient_411(struct s_server *ts, unsigned short sd, std::string command)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "411 ";
	reply += client + " ";
	reply += ":No recipient given (";
	reply += command + ")";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NOTEXTTOSEND (412)
void	err_notexttosend_412(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "412 ";
	reply += client + " ";
	reply += ":No text to send";
	send_reply(ts, sd, NULL, reply);
}

//ERR_UNKNOWNCOMMAND (421)
void	err_unknowncommand_421(struct s_server *ts, unsigned short sd, std::string command)
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
void	err_nonicknamegiven_431(struct s_server *ts, unsigned short sd)
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
void	err_erroneusnickname_432(struct s_server *ts, unsigned short sd, std::string nick)
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
void	err_nicknameinuse_433(struct s_server *ts, unsigned short sd, std::string nick)
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
void	err_usernotinchannel_441(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName)
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
void	err_notonchannel_442(struct s_server *ts, unsigned short sd, std::string channelName)
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

//ERR_USERONCHANNEL (443) 
void	err_useronchannel_443(struct s_server *ts, unsigned short sd, std::string nick, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "443 ";
	reply += client + " ";
	reply += nick + " ";
	reply += channelName + " ";
	reply += ":is already on channel";
	send_reply(ts, sd, NULL, reply);
}

//ERR_NEEDMOREPARAMS (461)
void	err_needmoreparams_461(struct s_server *ts, unsigned short sd, std::string command)
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
void	err_alreadyregistered_462(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "462 ";
	reply += client + " ";
	reply += ":You may not reregister";
	send_reply(ts, sd, NULL, reply);
}

//ERR_PASSWDMISMATCH (464)
void	err_passwdmismatch_464(struct s_server *ts, unsigned short sd)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "464 ";
	reply += client + " ";
	reply += ":Password incorrect";
	send_reply(ts, sd, NULL, reply);
}

//ERR_CHANNELISFULL (471)
void	err_channelisfull_471(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "471 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":Cannot join channel (+l)";
	send_reply(ts, sd, NULL, reply);
}

//ERR_INVITEONLYCHAN (473)
void	err_inviteonlychan_473(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "473 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":Cannot join channel (+i)";
	send_reply(ts, sd, NULL, reply);
}

//ERR_BADCHANNELKEY (475)
void	err_badchannelkey_475(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	std::string client;
	client = getClient(ts, sd);
	reply = "475 ";
	reply += client + " ";
	reply += channelName + " ";
	reply += ":Cannot join channel (+k)";
	send_reply(ts, sd, NULL, reply);
}

//ERR_BADCHANMASK (476)
void	err_badchanmask_476(struct s_server *ts, unsigned short sd, std::string channelName)
{
	std::string	reply;
	//std::string client;
	//client = getClient(ts, sd);
	reply = "476 ";
	//reply += client + " ";
	reply += channelName + " ";
	reply += ":Bad Channel Mask";
	send_reply(ts, sd, NULL, reply);
}

//ERR_CHANOPRIVSNEEDED (482)
void	err_chanopsprivsneeded_482(struct s_server *ts, unsigned short sd, std::string channelName)
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
void	err_unknownmodeflag_501(struct s_server *ts, unsigned short sd)
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
void	err_invalidmodeparam_696(struct s_server *ts, unsigned short sd, std::string target, std::string modeChar, std::string parameter, std::string description)
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