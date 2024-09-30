/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:16:21 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 15:23:05 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// MODE 
// MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on channel #Finnish.
// MODE #42 +k oulu                ; Set the channel key to "oulu".
// MODE #eu-opers +l 10            ; Set the limit for the number of users on channel to 10.
// MODE #Finnish +i               ; Makes #Finnish channel 'invite-only'.
//  t - topic settable by channel operator only flag;
//https://modern.ircdocs.horse/#mode-message
//https://modern.ircdocs.horse/#channel-modes


		// std::string		getKey(void);
		// void			setKey(std::string key);
		// unsigned int	getModeFlags(void);
		// void			andModeFlags(unsigned int mask);
		// void			orModeFlags(unsigned int mask);
		// int				getChannelLimit(void);
		// void			setChannelLimit(int channelLimit);
// 		  MODE #foobar
// Getting modes for a channel (and channel creation time):

//   :irc.example.com 324 dan #foobar +nrt
//   :irc.example.com 329 dan #foobar 1620807422
void	irc_mode(Message* msg, struct s_server *ts)
{
	std::string	reply;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	Channel *channel;
	std::string channelName = msg->getParams()[0];
	//403
	if (!isChannel(ts, channelName))
	{
		err_nosuchchannel_403(ts, msg->getSD(), channelName);
		return ;
	}
	channel = ts->channels[channelName];
	if (msg->getParams().size() < 2)
	{
		//324
		rpl_channelmodes_324(ts, msg->getSD(), channel);
		//329 (not yet)
		return ;
	}
	std::string					modestring = msg->getParams()[1];
	std::string					modestringReturn;
	std::vector<std::string>	paramsRetrun;
	size_t						n = 2;
	//501
	if (modestring.size() < 2 || (modestring[0] != '+' && modestring[0] != '-'))
	{
		err_unknownmodeflag_501(ts, msg->getSD());
		return ;
	}
	//442
	if (!(channel->hasUser(msg->getSD())))
	{
		err_notonchannel_442(ts, msg->getSD(), channelName);
	}
	//482
	if (!ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	// 400 incorrect string
	if (ok_containsDuplicate(modestring))
	{
		err_unknownerror_400(ts, msg->getSD(), msg->getCommand(), "Duplicate in the modestring");
		return ;
	}
	//USER MODE not handled

	//696 bad key or channel limit
	//check channel as param[0]
	//check plus or minus as param [1][0]
	//check param[1][1]
	bool	plus;
	bool	plus2;
	for (size_t i = 0; i < modestring.size(); i++)
	{
		if (modestring[i] == '+')
			plus = true;
		else if (modestring[i] == '-')
			plus = false;
		else if (modestring[i] == 'i')
		{
			if (plus)
			{
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " is now invite only.", "", MYDEBUG);
				channel->addModeFlags(CHANNEL_INVITE);
				if (!modestringReturn.size() || !plus2)
					modestringReturn += "+";
				plus2 = true;
			}
			else
			{
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " is no longer invite only.", "", MYDEBUG);
				channel->removeModeFlags(CHANNEL_INVITE);
				if (!modestringReturn.size() || plus2)
					modestringReturn += "-";
				plus2 = false;
			}
			modestringReturn += "i";
		}
		else if (modestring[i] == 't')
		{
			if (plus)
			{
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " topic is setable by moderators only.", "", MYDEBUG);
				channel->addModeFlags(CHANNEL_TOPIC);
				if (!modestringReturn.size() || !plus2)
					modestringReturn += "+";
				plus2 = true;
			}
			else
			{
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " topic is setable by anyone.", "", MYDEBUG);
				channel->removeModeFlags(CHANNEL_TOPIC);
				if (!modestringReturn.size() || plus2)
					modestringReturn += "-";
				plus2 = false;
			}
			modestringReturn += "t";
		}
		else if (modestring[i] == 'k')
		{
			if (plus)
			{
				if (msg->getParams().size() < n + 1 || !msg->getParams()[n].size())
				{
					err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
					break ;
				}
				if (!isValidKey(msg->getParams()[n]))
				{
					err_invalidmodeparam_696(ts, msg->getSD(), channelName, "k", msg->getParams()[n], "Invalid key");
					break ;
				}
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " locked by key:", msg->getParams()[n], MYDEBUG);
				channel->setKey(msg->getParams()[n++]);
				channel->addModeFlags(CHANNEL_KEY);
				if (!modestringReturn.size() || !plus2)
					modestringReturn += "+";
				plus2 = true;
				paramsRetrun.push_back("fake-key");
			}
			else
			{
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " unlocked.", "", MYDEBUG);
				channel->setKey("");
				channel->removeModeFlags(CHANNEL_KEY);
				if (!modestringReturn.size() || plus2)
					modestringReturn += "-";
				plus2 = false;
			}
			modestringReturn += "k";
		}
		else if (modestring[i] == 'l')
		{
			if (plus)
			{
				if (msg->getParams().size() < n + 1 || !msg->getParams()[n].size())
				{
					err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
					break ;
				}
				int	limit = ok_strtoi<int>(msg->getParams()[n++]);
				if (limit < 2 || limit > 64)
				{
					err_invalidmodeparam_696(ts, msg->getSD(), channelName, "l", msg->getParams()[n-1], "the limit has to be between 2 - 64 users per channel");
					break ;
				}
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " limited to " + ok_itostr(limit) + " users.", "", MYDEBUG);
				channel->setChannelLimit(limit);
				channel->addModeFlags(CHANNEL_LIMIT);
				if (!modestringReturn.size() || !plus2)
					modestringReturn += "+";
				plus2 = true;
				paramsRetrun.push_back(ok_itostr(limit));
			}
			else
			{
				ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " not limited.", "", MYDEBUG);
				channel->setChannelLimit(0);
				channel->removeModeFlags(CHANNEL_LIMIT);
				if (!modestringReturn.size() || plus2)
					modestringReturn += "-";
				plus2 = false;
			}
			modestringReturn += "l";
		}
		else if (modestring[i] == 'o')
		{
			if (msg->getParams().size() < n + 1)
			{
				err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
				break ;
			}
			std::string nick = msg->getParams()[n++];
			if (!isInChannel(ts, channelName, nick))
			{
				err_usernotinchannel_441(ts, msg->getSD(), nick, channelName);
				continue ;
			}
			if (plus)
			{
				ok_debugger(&(ts->debugger), DEBUG, ts->users[msg->getSD()]->getNick() + " added " + nick + " as an operator in the channel: " + channelName + "." , "", MYDEBUG);
				channel->addOperator(ts->nicks[nick]->getSD());
				if (!modestringReturn.size() || !plus2)
					modestringReturn += "+";
				plus2 = true;
			}
			else
			{
				ok_debugger(&(ts->debugger), DEBUG, ts->users[msg->getSD()]->getNick() + " removed " + nick + " from operators in the channel: " + channelName + "." , "", MYDEBUG);
				channel->removeOperator(ts->nicks[nick]->getSD());
				if (!modestringReturn.size() || plus2)
					modestringReturn += "-";
				plus2 = false;
			}
			modestringReturn += "o";
			paramsRetrun.push_back(nick);
		}
		else
		{
			err_unknownmodeflag_501(ts, msg->getSD());
			break ;
		}
	}
	reply = "MODE ";
	reply += channelName + " ";
	reply += modestringReturn;
	for (size_t i = 0; i < paramsRetrun.size(); i++)
		reply += " " + paramsRetrun[i];
	send_reply_channel(ts, channelName, ts->users[msg->getSD()], reply);
}
