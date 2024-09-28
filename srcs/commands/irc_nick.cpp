/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:48:33 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:49:35 by tlukanie         ###   ########.fr       */
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
void    irc_nick(Message* msg, struct s_server *ts)
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
	ts->users[msg->getSD()]->setNick(newnick);
	if (oldnick.size())
	{
		ts->nicks.erase(ts->nicks.find(oldnick));
	}
	ts->nicks.insert(std::pair<std::string, User*>(newnick, ts->users[msg->getSD()]));
	if (ts->users[msg->getSD()]->getNick().size())
	{
		//send to all?
		//:net!net@127.0.0.1 << need to be not hardcoded
		reply = ":" + oldnick + "!" + ts->users[msg->getSD()]->getUserName() + "@" + ts->users[msg->getSD()]->getIP();
		reply += " NICK :" +  ts->users[msg->getSD()]->getNick();
		send_reply(ts, msg->getSD(), NULL, reply);
	}
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