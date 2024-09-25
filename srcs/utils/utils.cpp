/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 13:37:36 by okraus            #+#    #+#             */
/*   Updated: 2024/09/25 12:27:49 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"


int	ft_usage(void)
{
	std::cerr << ERROR_COLOUR "Usage:" NO_COLOUR << std::endl
		<< REDBG_COLOUR "./ircserv <port> <password> --debug-lvl=n (n = 0-4)" NO_COLOUR << std::endl;
	return (1);
}

int	ft_usage_port(void)
{
	std::cerr << ERROR_COLOUR "Invalid port:" NO_COLOUR << std::endl
		<< REDBG_COLOUR "port has to be in the range of 1-65535" NO_COLOUR << std::endl;
	return (2);
}

int	ft_usage_debug(void)
{
	std::cerr << ERROR_COLOUR "Invalid debug message:" NO_COLOUR << std::endl
		<< REDBG_COLOUR "--debug-lvl=n; n has to be integer between 0 and 4" NO_COLOUR << std::endl;
	return (2);
}

size_t	ok_crlf_finder(std::vector<uint8_t> data)
{
	size_t							pos;
	std::vector<uint8_t>::iterator	temp;

	pos = 1;
	for (std::vector<uint8_t>::iterator it = data.begin(); it != data.end(); it++)
	{
		++pos;
		temp = it;
		temp++;
		if (temp != data.end() && *it == '\r' && *temp == '\n')
			return (pos);
	}
	return (0);
}

bool	isChannel(struct s_server *ts, std::string channelName)
{
	return (ts->channels.find(channelName) != ts->channels.end());
}

bool	isNick(struct s_server *ts, std::string nick)
{
	return (ts->nicks.find(nick) != ts->nicks.end());
}

bool	isInChannel(struct s_server *ts, std::string channelName, std::string nick)
{
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		if (it->second->getNick() == nick)
			return (true);
	}
	return (false);
}

void	send_reply(struct s_server *ts, unsigned short sd, User *Sender, std::string text)
{
	std::string	reply;

	if (ts->users[sd]->getFreedom())
		return ;
	if (Sender)
		reply += ":" + Sender->getNick() + "!" + Sender->getUserName() + "@" + Sender->getIP() + " ";
	reply += text + CRLF;
	ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(sd) + "]", ok_display_reply(&(ts->debugger), reply), MYDEBUG);
	for (std::string::iterator it = reply.begin(); it != reply.end(); it++)
	{
		ts->users[sd]->_data_out.push_back(*it);
	}
}

void	send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text)
{
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		send_reply(ts, it->second->getSD(), Sender, text);
	}
}

int	remove_user_from_channel(struct s_server *ts, User *user, Channel *channel)
{
	std::string channelName = channel->getChannelName();
	//remove user from multimap
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		if (it->second->getNick() == user->getNick())
		{
			ok_debugger(&(ts->debugger), NOTICE, "Deleting user: ", it->second->getNick(), MYDEBUG);
			ts->channel2user.erase(it);
			break ;
		}
	}
	// ts->user2channel.insert(std::pair<std::string, Channel*>(user->getNick(), ts->channels[channelName]));
	//remove channel from multimap
	for (std::multimap<std::string, Channel*>::iterator it = ts->user2channel.lower_bound(user->getNick()); it != ts->user2channel.upper_bound(user->getNick()); it++)
	{
		if (it->second->getChannelName() == channelName)
		{
			ok_debugger(&(ts->debugger), NOTICE, "Erasing channel for user: ", channelName, MYDEBUG);
			ts->user2channel.erase(it);
			break ;
		}
	}
	// ts->channel2user.insert(std::pair<std::string, User*>(channelName, user));
	//if no users in channel delete it
	if (channel->removeUser(user->getSD()) == 0)
	{
		delete ts->channels[channelName];
		ts->channels.erase(channelName);
		ok_debugger(&(ts->debugger), INFO, "Deleting channel: ", channelName, MYDEBUG);
	}
	return (0);
}

int	remove_user_from_server(struct s_server *ts, User *user, std::string reason)
{
	user->setFree();
	std::string	reply;

	reply = "QUIT";
	if (reason.size())
		reply += " :" + reason;
	for (std::multimap<std::string, Channel*>::iterator it = ts->user2channel.lower_bound(user->getNick()); it != ts->user2channel.upper_bound(user->getNick());)
	{
		std::multimap<std::string, Channel*>::iterator temp = it++;
		send_reply_channel(ts, temp->second->getChannelName(), user, reply);
		remove_user_from_channel(ts, user, temp->second);
	}
	return (0);
}

int	add_user_to_channel(struct s_server *ts, User *user, Channel *channel)
{
	//add channel to multimap
	ts->user2channel.insert(std::pair<std::string, Channel*>(user->getNick(), channel));
	//add user to multimap
	ts->channel2user.insert(std::pair<std::string, User*>(channel->getChannelName(), user));
	if (channel->addUser(user->getSD()) == 1)
		channel->addOperator(user->getSD());
	return (0);
}


// void	send_reply_others(struct s_server *ts, std::string channelName, User *Sender, std::string text)
// {
// 	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
// 	{
// 		if (it->second->getNick() != ts->users[Sender->getSD()]->getNick())
// 		{
// 			send_reply(ts, it->second->getSD(), Sender, text);
// 		}
// 	}
// }


bool	isValidNick(std::string	nick)
{
	std::string validFirst = LOWER UPPER SPECIAL;
	std::string	validRest = validFirst + "-" DIGITS;
	if (!nick.size() || nick.size() > 32)
		return (false);
	if (validFirst.find(nick[0]) == std::string::npos)
		return (false);
	for (size_t i = 1; i < nick.size(); i++)
	{
		if (validRest.find(nick[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

bool	isValidKey(std::string	key)
{
	std::string valid = KEY;
	if (!key.size() || key.size() > 32)
		return (false);
	for (size_t i = 0; i < key.size(); i++)
	{
		if (valid.find(key[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

std::vector<std::string>	ok_split(std::string str, char c)
{
	std::string line;
	std::vector<std::string> strings;
	std::stringstream ss(str);
	while(std::getline(ss, line, c))
	{
		strings.push_back(line);
	}
	return strings;
}


bool	ok_containsDuplicate(const std::string &text)
{
	std::string	copy = text;
	size_t		size = text.size();
	if (size > 256)
		size = 256;
	for	(size_t i = 0; i < size; i++)
	{
		for	(size_t j = 0; j < size; j++)
		{
			if (i != j && copy[i] == text[j])
				return (true);
		}
	}
	return (false);
}
