/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:48:33 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/30 10:00:16 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// NICK net
//CHECK IF NICK IS UNIQUE
// https://modern.ircdocs.horse/#nick-message
// handle existing nick later
// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

// Numeric Replies:

// ERR_NONICKNAMEGIVEN (431)
// ERR_ERRONEUSNICKNAME (432)
// ERR_NICKNAMEINUSE (433)
// ERR_NICKCOLLISION (436) //not needed by us
// Command Example:


// oldnickname[ [ "!" user ] "@" host ] 
// oldnick!user@host
//   NICK Wiz                  ; Requesting the new nick "Wiz".
// Message Examples:

//   :WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.

//   :dan-!d@localhost NICK Mamoped
//                             ; dan- changed his nickname to Mamoped.
void	irc_nick(Message* msg, struct s_server *ts)
{
	std::string	reply;
	std::string	oldnick;

	if (!(ts->users[msg->getSD()]->getAuthFlags() & PASSWORD))
		return ;
	if (ts->users[msg->getSD()]->getNick().size())
		oldnick = ts->users[msg->getSD()]->getNick();
	//check if parameter
	if (!msg->getParams().size())
	{
		err_nonicknamegiven_431(ts, msg->getSD());
		return ;
	}
	std::string	newnick = msg->getParams()[0];
	if (!isValidNick(newnick))
	{
		err_erroneusnickname_432(ts, msg->getSD(), newnick);
		return ;
	}
	if (ts->nicks.find(newnick) != ts->nicks.end())
	{
		err_nicknameinuse_433(ts, msg->getSD(), newnick);
		return ;
	}
	reply = "NICK :" +  newnick;
	if (ts->users[msg->getSD()]->getNick().size())
	{
		//send to all?
		//:net!net@127.0.0.1 << need to be not hardcoded
		//iterate over all users
		//iterate over all channels the user is in
		//check if user changing the nick is in any of them
		for (std::map<int, User*>::iterator it = ts->users.begin(); it != ts->users.end(); it++)
		{
			if (it->first == msg->getSD())
				continue ;
			for (std::multimap<std::string, Channel*>::iterator iter = ts->user2channel.lower_bound(it->second->getNick()); iter != ts->user2channel.upper_bound(it->second->getNick()); iter++)
			{
				if (isInChannel(ts, iter->second->getChannelName(), ts->users[msg->getSD()]->getNick()))
				{
					send_reply(ts, it->second->getSD(), ts->users[msg->getSD()], reply);
					break ;
				}
			}
		}
	}
	send_reply(ts, msg->getSD(), ts->users[msg->getSD()], reply);
	ts->users[msg->getSD()]->setNick(newnick);
	if (oldnick.size())
	{
		ts->nicks.erase(ts->nicks.find(oldnick));
	}
	ts->nicks.insert(std::pair<std::string, User*>(newnick, ts->users[msg->getSD()]));
	if (!oldnick.size())
	{
		ts->users[msg->getSD()]->addAuthFlag(NICK);
		if (ts->users[msg->getSD()]->getAuthFlag())
		{
			rpl_welcome_001(ts, msg->getSD());
		}
	}
	if (!oldnick.size())
	{
		oldnick = ts->users[msg->getSD()]->getIP() + ":" + ok_itostr(ts->users[msg->getSD()]->getPort());
	}
	ok_debugger(&(ts->debugger), INFO, "[" + ok_itostr(msg->getSD()) + "] User: " + oldnick + " got nick " + ts->users[msg->getSD()]->getNick() + ".", "", MYDEBUG);
}