/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:16:21 by okraus            #+#    #+#             */
/*   Updated: 2024/09/26 13:48:40 by tlukanie         ###   ########.fr       */
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
void irc_mode(Message* msg, struct s_server *ts)
{
	std::string	reply;
	std::cout << MAGENTA_COLOUR "MODE COMMAND not really supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		// reply = "461 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += "MODE :Not enough parameters";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	Channel *channel;
	std::string channelName = msg->getParams()[0];
	//403
	if (!isChannel(ts, channelName))
	{
		// reply = "403 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += channelName + " ";
		// reply += ":No such channel";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_403(ts, msg->getSD(), channelName);
		return ;
	}
	channel = ts->channels[channelName];
	if (msg->getParams().size() < 2)
	{
		//324
		// reply = "324 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += channelName + " ";
		// if (channel->getModeFlags())
		// 	reply += "+";
		// if (channel->getModeFlags() & CHANNEL_INVITE)
		// 	reply += "i";
		// if (channel->getModeFlags() & CHANNEL_TOPIC)
		// 	reply += "t";
		// if (channel->getModeFlags() & CHANNEL_LIMIT)
		// 	reply += "l";
		// if (channel->getModeFlags() & CHANNEL_KEY)
		// 	reply += "k";
		// if (channel->getModeFlags() & CHANNEL_LIMIT)
		// 	reply += " " + ok_itostr(channel->getChannelLimit());
		// if (channel->getModeFlags() & CHANNEL_KEY)
		// 	reply += " fake_key";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_324(ts, msg->getSD(), channel);
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
		// reply = "501 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += ":Unknown MODE flag";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_501(ts, msg->getSD());
		return ;
	}
	//442
	if (!(channel->hasUser(msg->getSD())))
	{
		// reply = "442 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += channelName + " ";
		// reply += ":You are not on that channel";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_442(ts, msg->getSD(), channelName);
	}
	//482
	if (!ts->channels[channelName]->isOperator(msg->getSD()))
	{
		// reply = "482 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += channelName + " ";
		// reply += ":You are not channel operator";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_482(ts, msg->getSD(), channelName);
		return ;
	}
	// 400 incorrect string
	if (ok_containsDuplicate(modestring))
	{
		// reply = "400 ";
		// reply += ts->users[msg->getSD()]->getNick() + " ";
		// reply += "MODE :Duplicate in the modestring";
		// send_reply(ts, msg->getSD(), NULL, reply);
		tl_send_400(ts, msg->getSD(), msg->getCommand(), "Duplicate in the modestring");
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
					// reply = "461 ";
					// reply += ts->users[msg->getSD()]->getNick() + " ";
					// reply += "MODE :Not enough parameters";
					// send_reply(ts, msg->getSD(), NULL, reply);
					tl_send_461(ts, msg->getSD(), msg->getCommand());
					break ;
				}
				if (!isValidKey(msg->getParams()[n]))
				{
					// reply = "696 ";
					// reply += ts->users[msg->getSD()]->getNick() + " ";
					// reply += channelName + " ";
					// reply += "k ";
					// reply += msg->getParams()[n] + " ";
					// reply += ":Invalid key";
					// send_reply(ts, msg->getSD(), NULL, reply);
					ok_send_696(ts, msg->getSD(), channelName, "k", msg->getParams()[n], "Invalid key");
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
					// reply = "461 ";
					// reply += ts->users[msg->getSD()]->getNick() + " ";
					// reply += "MODE :Not enough parameters";
					// send_reply(ts, msg->getSD(), NULL, reply);
					tl_send_461(ts, msg->getSD(), msg->getCommand());
					break ;
				}
				int	limit = ok_strtoi<int>(msg->getParams()[n++]);
				if (limit < 2 || limit > 64)
				{
					// reply = "696 ";
					// reply += ts->users[msg->getSD()]->getNick() + " ";
					// reply += channelName + " ";
					// reply += "l ";
					// reply += msg->getParams()[n - 1] + " ";
					// reply += ":the limit has to be between 2 - 64 users per channel";
					// send_reply(ts, msg->getSD(), NULL, reply);
					ok_send_696(ts, msg->getSD(), channelName, "l", msg->getParams()[n-1], "the limit has to be between 2 - 64 users per channel");
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
				// reply = "461 ";
				// reply += ts->users[msg->getSD()]->getNick() + " ";
				// reply += "MODE :Not enough parameters";
				// send_reply(ts, msg->getSD(), NULL, reply);
				tl_send_461(ts, msg->getSD(), msg->getCommand());
				break ;
			}
			std::string nick = msg->getParams()[n++];
			if (!isInChannel(ts, channelName, nick))
			{
				// reply = "441 ";
				// reply += ts->users[msg->getSD()]->getNick() + " ";
				// reply += nick + " ";
				// reply += channelName + " ";
				// reply += ":They are not on that channel";
				// send_reply(ts, msg->getSD(), NULL, reply);
				tl_send_441(ts, msg->getSD(), nick, channelName);
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
			// reply = "501 ";
			// reply += ts->users[msg->getSD()]->getNick() + " ";
			// reply += ":Unknown MODE flag";
			// send_reply(ts, msg->getSD(), NULL, reply);
			tl_send_501(ts, msg->getSD());
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
