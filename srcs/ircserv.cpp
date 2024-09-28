/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:16:33 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 13:08:48 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket users with select and fd_set on Linux 

#include "../includes/ircserv.hpp"

static bool		g_server_alive = true;

void signal_handler(int signal_num) 
{
	if (signal_num == SIGINT)
		g_server_alive = false;
}


// CAP LS
void irc_cap(Message* msg, struct s_server *ts)
{
	(void)msg;
	(void)ts;
}

// PASS a
// https://modern.ircdocs.horse/#pass-message
// Numeric replies:
void irc_pass(Message* msg, struct s_server *ts)
{
	std::string reply;

	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	// ERR_ALREADYREGISTERED (462)
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		err_alreadyregistered_462(ts, msg->getSD());
		return ;
	}
	if (msg->getParams()[0] == ts->password)
	{
		ts->users[msg->getSD()]->addAuthFlag(PASSWORD);
	}
	else
	{
		// ERR_PASSWDMISMATCH (464)
		reply = "464  :Password incorrect";
		send_reply(ts, msg->getSD(), NULL, reply);
		// possibly count number of incorrect tries
	}
	
}


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
void irc_nick(Message* msg, struct s_server *ts)
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

// USER net net localhost :net
// USER guest 0 * :Ronnie Reagan
//Parameters: <username> <hostname> <servername> <realname> //hostname and servername can be ignored for local users
// https://www.rfc-editor.org/rfc/rfc2812#section-3.1.3
// Parameters: <user> <mode> <unused> <realname>
//      Command: USER
//   Parameters: <username> 0 * <realname>
// https://modern.ircdocs.horse/#user-message
// reply :IRCQ+ 001 net :Welcome to IRCQ+ net!net@127.0.0.1
//net!net@127.0.0.1 <<<< save this string somehow? Create function to make it?
//Numeric Replies:

// ERR_NEEDMOREPARAMS (461)
// ERR_ALREADYREGISTERED (462)
void irc_user(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlags() & PASSWORD))
		return ;
	// we need to check that there are 4 parameters?
	if (msg->getParams().size() != 4 || !msg->getParams()[0].size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		err_alreadyregistered_462(ts, msg->getSD());
		return ;
	}
	ts->users[msg->getSD()]->setUserName(msg->getParams()[0]);
	ts->users[msg->getSD()]->setHostName(msg->getParams()[1]);
	ts->users[msg->getSD()]->setServerName(msg->getParams()[2]);
	ts->users[msg->getSD()]->setRealName(msg->getParams()[3]);
	// process the 4 parameters
	ts->users[msg->getSD()]->addAuthFlag(USER);
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		rpl_welcome_001(ts, msg->getSD());
	}
}

// PING
// https://modern.ircdocs.horse/#ping-message
// :IRCQ+ PONG net :IRCQ+
// Numeric Replies:

// ERR_NEEDMOREPARAMS (461)
// ERR_NOORIGIN (409)
void irc_ping(Message* msg, struct s_server *ts)
{
	std::string	reply;

	//this check may not be needed
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	reply = "PONG " +  ts->users[msg->getSD()]->getNick() + " :" + msg->getParams()[0];
	send_reply(ts, msg->getSD(), NULL, reply);
}

// PONG
// https://modern.ircdocs.horse/#pong-message
void irc_pong(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
}

// Error
// https://modern.ircdocs.horse/#error-message
// QUIT
// :dan-!d@localhost QUIT :Quit: Bye for now!
// https://modern.ircdocs.horse/#quit-message
void irc_quit(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	std::string	reason;
	reason = "Quit: ";
	if (msg->getParams().size())
		reason += msg->getParams()[0];
	reply = "ERROR :Quitting channel";
	send_reply(ts, msg->getSD(), NULL, reply);
	//send quitting message to all uers that share the channels
	//user leave all the channels they are in
	remove_user_from_server(ts, ts->users[msg->getSD()], reason);
}

/* CHANNEL OPERATIONS */

// JOIN
// https://modern.ircdocs.horse/#join-message
void irc_join(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	//special case
	if (msg->getParams()[0] == "0")
	{
		//leave all channels
		for (std::multimap<std::string, Channel*>::iterator it = ts->user2channel.lower_bound(ts->users[msg->getSD()]->getNick()); it != ts->user2channel.upper_bound(ts->users[msg->getSD()]->getNick()); /*Iterating in the loop*/)
		{
			std::multimap<std::string, Channel*>::iterator temp;
			temp = it++;
			reply += "PART " + temp->second->getChannelName() + " :leaving";
			//send reply to channel
			send_reply_channel(ts, temp->second->getChannelName(), ts->users[msg->getSD()], reply);
			remove_user_from_channel(ts, ts->users[msg->getSD()], ts->channels[temp->second->getChannelName()]);
		}
		return ;
	}
	std::vector<std::string>	channelNames = ok_split(msg->getParams()[0], ',');
	std::vector<std::string>	keys;
	if (msg->getParams().size() > 1)
		keys =  ok_split(msg->getParams()[1], ',');
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		//check if the channel name valid
		if (channelNames[i].size() < 2 || (channelNames[i][0] != '#' && channelNames[i][0] != '&'))
		{
			err_badchanmask_476(ts, msg->getSD(), channelNames[i]);
			continue ;
		}
		//if the channel does not exist
		if (ts->channels.find(channelNames[i]) == ts->channels.end())
		{
			try
			{
				ts->channels[channelNames[i]] = new Channel(channelNames[i], msg->getSD());
				ok_debugger(&(ts->debugger), INFO, "Creating channel: ", channelNames[i], MYDEBUG);
			}
			catch(const std::exception& e)
			{
				ok_debugger(&(ts->debugger), ERROR, "Could not create channel: " + channelNames[i] + ", because: ", e.what(), MYDEBUG);
				return ;
			}
		}
		else
		{
			//check if user can join the channel
			if (ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_INVITE)
			{
				err_inviteonlychan_473(ts, msg->getSD(), channelNames[i]);
				continue ;
			}
			if ((ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_LIMIT) && ts->channels[channelNames[i]]->getUsers() >= ts->channels[channelNames[i]]->getChannelLimit())
			{
				err_channelisfull_471(ts, msg->getSD(), channelNames[i]);
				continue ;
			}
			if ((ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_KEY))
			{
				if (keys.size() <= i || keys[i] != ts->channels[channelNames[i]]->getKey())
				{
					err_badchannelkey_475(ts, msg->getSD(), channelNames[i]);
					continue ;
				}
			}
		}
		add_user_to_channel(ts, ts->users[msg->getSD()], ts->channels[channelNames[i]]);
		//send everyone join message
		ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(msg->getSD()) + "] Joining channel:", channelNames[i], MYDEBUG);
		reply = "JOIN :" + channelNames[i];
		send_reply_channel(ts, channelNames[i], ts->users[msg->getSD()], reply);
		//332
		if (ts->channels[channelNames[i]]->getTopic().size())
		{
			rpl_topic_332(ts, msg->getSD(), channelNames[i]);
		}
		rpl_namreply_353(ts, msg->getSD(), channelNames[i]);
		rpl_endofnames_366(ts, msg->getSD(), channelNames[i]);
	}
}

// PART
//https://modern.ircdocs.horse/#part-message
void irc_part(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::vector<std::string>	channelNames = ok_split(msg->getParams()[0], ',');
	std::string					reason;
	if (msg->getParams().size() > 1)
		reason = msg->getParams()[1];
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		//if the channel does not exist
		if (ts->channels.find(channelNames[i]) == ts->channels.end())
		{
			err_nosuchchannel_403(ts, msg->getSD(), channelNames[i]);
			continue ;
		}
		if (!(ts->channels[channelNames[i]]->hasUser(msg->getSD())))
		{
			err_notonchannel_442(ts, msg->getSD(), channelNames[i]);
			continue ;
		}
		ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(msg->getSD()) + "] Leaving channel:", channelNames[i], MYDEBUG);
		reply += "PART " + channelNames[i] + " :";
		reply += (reason.size() ? reason : std::string(ts->users[msg->getSD()]->getNick() + " leaving channel " + channelNames[i]));
		//send reply to channel
		send_reply_channel(ts, channelNames[i], ts->users[msg->getSD()], reply);
		remove_user_from_channel(ts, ts->users[msg->getSD()], ts->channels[channelNames[i]]);
	}
}

//TOPIC
// https://modern.ircdocs.horse/#topic-message
//TOPIC #test :Hello
//:user123!net@127.0.0.1 TOPIC #test :Hello

void irc_topic(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	//send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text, int flags)
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string channelName = msg->getParams()[0];
	std::string topic;
	if (msg->getParams().size() > 1)
		topic = msg->getParams()[1];

	if (!isChannel(ts, channelName))
	{
		err_nosuchchannel_403(ts, msg->getSD(), channelName);
		return ;
	}
	if (!(ts->channels[channelName]->hasUser(msg->getSD())))
	{
		err_notonchannel_442(ts, msg->getSD(), channelName);
		return ;
	}
	if (msg->getParams().size() == 1)
	{
		if (ts->channels[channelName]->getTopic().size())
		{
			rpl_topic_332(ts, msg->getSD(), channelName);
			return ;
		}
		else
		{
			rpl_notopic_331(ts, msg->getSD(), channelName);
			return ;
		}
	}
	// setting topic
	//482
	if ((ts->channels[channelName]->getModeFlags() & CHANNEL_TOPIC) && !ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	reply = "TOPIC " + channelName + " :" + topic;
	ts->channels[channelName]->setTopic(topic);
	send_reply_channel(ts, channelName, ts->users[msg->getSD()], reply);
}

//INVITE
// https://modern.ircdocs.horse/#invite-message
// INVITE user123 #test
// FD:5	:IRCQ+ 341 net user123 #test
// FD:5	:user123!net@127.0.0.1 JOIN :#test
// FD:6	:user123!net@127.0.0.1 JOIN :#test
// FD:6	:IRCQ+ 332 user123 #test :
// FD:6	:IRCQ+ 353 user123 = #test :@net user123
// FD:6	:IRCQ+ 366 user123 #test :End of Names list

void irc_invite(Message* msg, struct s_server *ts)
{
	std::string reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	// if user exists and if user not in channel, join user to channel
	if (msg->getParams().size() < 2)
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string nick = msg->getParams()[0];
	std::string channelName = msg->getParams()[1];
	//401
	if (ts->nicks.find(nick) == ts->nicks.end())
	{
		err_nosuchnick_401(ts, msg->getSD(), nick);
		return ;
	}
	//403
	if (!isChannel(ts, channelName))
	{
		err_nosuchchannel_403(ts, msg->getSD(), channelName);
		return ;
	}
	//442
	if (!(ts->channels[channelName]->hasUser(msg->getSD())))
	{
		err_notonchannel_442(ts, msg->getSD(), channelName);
		return ;
	}
	//443
	if (ts->channels[channelName]->hasUser(ts->nicks[nick]->getSD()))
	{
		err_useronchannel_443(ts, msg->getSD(), nick, channelName);
		return ;
	}
	//482
	if ((ts->channels[channelName]->getModeFlags() & CHANNEL_INVITE) && !ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	if ((ts->channels[channelName]->getModeFlags() & CHANNEL_LIMIT) && ts->channels[channelName]->getUsers() >= ts->channels[channelName]->getChannelLimit())
	{
		err_channelisfull_471(ts, msg->getSD(), channelName);
		return ;
	}
	//341
	rpl_inviting_341(ts, msg->getSD(), nick, channelName);
	//invite message
	reply = "INVITE ";
	reply += nick + " ";
	reply += channelName;
	send_reply(ts, ts->nicks[nick]->getSD(), ts->users[msg->getSD()], reply);
	// not sure about the part below?
	reply = "JOIN :" + channelName;
	add_user_to_channel(ts, ts->nicks[nick], ts->channels[channelName]);
	send_reply_channel(ts, channelName, ts->nicks[nick], reply);
	// send_reply(ts, ts->nicks[nick]->getSD(), ts->nicks[nick], reply);
}

//KICK
//https://modern.ircdocs.horse/#kick-message
//  KICK #Finnish John :Speaking English
//    :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
void irc_kick(Message* msg, struct s_server *ts)
{
	std::string reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	// if user exists and if user not in channel, join user to channel
	if (msg->getParams().size() < 2)
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string					channelName = msg->getParams()[0];
	std::vector<std::string>	nicks = ok_split(msg->getParams()[1], ',');
	std::string reason;
	//403
	if (!isChannel(ts, channelName))
	{
		err_nosuchchannel_403(ts, msg->getSD(), channelName);
		return ;
	}
	//442
	if (!(ts->channels[channelName]->hasUser(msg->getSD())))
	{
		err_notonchannel_442(ts, msg->getSD(), channelName);
		return ;
	}
	//482
	if (!ts->channels[channelName]->isOperator(msg->getSD()))
	{
		err_chanopsprivsneeded_482(ts, msg->getSD(), channelName);
		return ;
	}
	for (size_t i = 0; i < nicks.size(); i++)
	{
		//401
		if (ts->nicks.find(nicks[i]) == ts->nicks.end())
		{
			err_nosuchnick_401(ts, msg->getSD(), nicks[i]);
			continue ;
		}
		//441
		if (!(ts->channels[channelName]->hasUser(ts->nicks[nicks[i]]->getSD())))
		{
			err_usernotinchannel_441(ts, msg->getSD(), nicks[i], channelName);
			continue ;
		}
		reply = "KICK " + channelName + " " + nicks[i];
		if (msg->getParams().size() > 2)
			reply += " :" + msg->getParams()[2];
		send_reply_channel(ts, channelName, ts->users[msg->getSD()], reply);
		remove_user_from_channel(ts, ts->nicks[nicks[i]], ts->channels[channelName]);
	}
}

//AWAY
//https://modern.ircdocs.horse/#away-message
void irc_away(Message* msg, struct s_server *ts)
{
	//not supported yet
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		ts->users[msg->getSD()]->setAwayMessage("");
		//reply unwaway
		send_reply(ts, msg->getSD(), NULL, "305 " + ts->users[msg->getSD()]->getNick() + " :You are no longer marked as being away");
	}
	else if (msg->getParams().size() == 1)
	{
		ts->users[msg->getSD()]->setAwayMessage(msg->getParams()[0]);
		//reply nowaway
		send_reply(ts, msg->getSD(), NULL, "306 " + ts->users[msg->getSD()]->getNick() + " :You have been marked as being away");
	}
}

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there
//not sending messages to chanops only, yet...

void irc_privmsg(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_norecipient_411(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	if (msg->getParams().size() < 2)
	{
		err_notexttosend_412(ts, msg->getSD());
		return ;
	}
	std::vector<std::string>	targets = ok_split(msg->getParams()[0], ',');
	std::string					text = msg->getParams()[1];

	for (size_t i = 0; i < targets.size(); i++)
	{
		reply = "PRIVMSG " + targets[i] + " :" + text;
		if (!targets[i].size())
		{
			err_norecipient_411(ts, msg->getSD(), msg->getCommand());
			continue ;
		}
		if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '&')
		{
			//403
			if (ts->channels.find(targets[i]) == ts->channels.end())
			{
				err_nosuchchannel_403(ts, msg->getSD(), targets[i]);
				continue ;
			}
			send_reply_channel(ts, targets[i], ts->users[msg->getSD()], reply);
		}
		else
		{
			//401
			if (ts->nicks.find(targets[i]) == ts->nicks.end())
			{
				err_nosuchnick_401(ts, msg->getSD(), targets[i]);
				continue ;
			}
			send_reply(ts, ts->nicks[targets[i]]->getSD(), ts->users[msg->getSD()], reply);
		}
	}
}

//NOTICE
//https://modern.ircdocs.horse/#notice-message

void irc_notice(Message* msg, struct s_server *ts)
{
	std::string	reply;

	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		//411
		return ;
	}
	if (msg->getParams().size() < 2)
	{
		//412
		return ;
	}
	std::vector<std::string>	targets = ok_split(msg->getParams()[0], ',');
	std::string					text = msg->getParams()[1];

	for (size_t i = 0; i < targets.size(); i++)
	{
		reply = "NOTICE " + targets[i] + " :" + text;
		if (!targets[i].size())
		{
			//411
			continue ;
		}
		if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '&')
		{
			//403
			if (ts->channels.find(targets[i]) == ts->channels.end())
			{
				continue ;
			}
			send_reply_channel(ts, targets[i], ts->users[msg->getSD()], reply);
		}
		else
		{
			//401
			if (ts->nicks.find(targets[i]) == ts->nicks.end())
			{
				continue ;
			}
			send_reply(ts, ts->nicks[targets[i]]->getSD(), ts->users[msg->getSD()], reply);
		}
	}
}

//WHO
// https://modern.ircdocs.horse/#who-message
void irc_who(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	std::string	mask = msg->getParams()[0];
	if (mask[0] == '#' || mask[0] == '&')
	{
		if (isChannel(ts, mask))
		{
			for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(mask); it != ts->channel2user.upper_bound(mask); it++)
			{
				rpl_whoreply_352(ts, ts->users[msg->getSD()]->getNick(), mask, it->second->getNick());
			}
		}
	}
	else
	{
		if (isNick(ts, mask))
		{
			rpl_whoreply_352(ts, ts->users[msg->getSD()]->getNick(), "", mask);
		}
	}
	rpl_endofwho_315(ts, ts->users[msg->getSD()]->getNick(), mask);
}

//WHOIS == DEBUG
void irc_whois(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		//461??
		//add reply error
		return ;
	}
	if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '&')
	{
		std::string channelName = msg->getParams()[0];
		if (ts->channels.find(channelName) != ts->channels.end())
			ts->debugger.log += ts->channels[channelName]->print(ts->debugger.colour);
	}
	else
	{
		std::string nick = msg->getParams()[0];
		if (ts->nicks.find(nick) != ts->nicks.end())
			ts->debugger.log += ts->nicks[nick]->print(ts->debugger.colour);
	}
	//if not a channel
		//no such nick 401
	//if channel
		//402
}


void	server_loop(t_server *ts)
{
	//set of socket descriptors 
	fd_set readfds;
	fd_set writefds;
	// //NEEDED TO DO EVERY LOOP BECAUSE OF SELECT
	// FD_ZERO(&readfds);
	// FD_ZERO(&writefds); 
	// FD_SET(ts.master_socket, &readfds);
	while(g_server_alive) 
	{
		// if (DEEPDEBUG)
		// 	ok_debugger(&(ts->debugger), DEBUG, "While loop started...", "", MYDEBUG);
		//clear the socket set 
		FD_ZERO(&readfds);
		FD_ZERO(&writefds); 
		//add master socket to set 
		FD_SET(ts->master_socket, &readfds); 
		ts->max_sd = ts->master_socket; 

		if (ts->debugger.log.size())
		{
			FD_SET(ts->debugger.fd, &writefds);
			if(ts->debugger.fd > ts->max_sd)
				ts->max_sd = ts->debugger.fd;
		}
		//add connection sockets to set
		//in the reading loop
		// if (ts->state & READING_LOOP)
		// {
		User * connection_ptr;
		for (std::map<int, User *>::iterator it = ts->users.begin(); it != ts->users.end(); it++)
		{
			//socket descriptor
			ts->sd = it->first;
			connection_ptr = it->second;
			// if (DEEPDEBUG)
			// 	ok_debugger(&(ts->debugger), DEBUG, "First for loop reading sd ", ok_itostr(ts->sd), MYDEBUG);
			//if something to send, move to writefds, otherwise be ready to read
			if (connection_ptr->getDataOut().size())
			{
				FD_SET(ts->sd , &writefds);
				// if (DEEPDEBUG)
				// 	ok_debugger(&(ts->debugger), DEBUG, "Setting reading sd: ", ok_itostr(ts->sd), MYDEBUG);
			}
			else
			{
				FD_SET(ts->sd , &readfds);
				// if (DEEPDEBUG)
				// 	ok_debugger(&(ts->debugger), DEBUG, "Setting writing sd: ", ok_itostr(ts->sd), MYDEBUG);
			}
			if(ts->sd > ts->max_sd)
				ts->max_sd = ts->sd;
		}
		// ok_debugger(&(ts->debugger), DEBUG, "Before select", "", MYDEBUG);
		//function to find nfds goes here
		// MAX(ts->master_socket, CONNECTIONS-highest key) + 1
		// nfds   This argument should be set to the highest-numbered file 
		// descriptor in any of the three sets, plus 1. The indicated file
		// descriptors in each set are checked, up to this limit (but see
		// BUGS).
		ts->timeout.tv_sec = 15;
		ts->timeout.tv_usec = 0;
		ts->activity = select(ts->max_sd + 1, &readfds , &writefds , NULL , &ts->timeout);
		// ok_debugger(&(ts->debugger), DEBUG, "After select", "", MYDEBUG);
		//check later if allowed
		if (ts->activity < 0) 
		{ 
			ok_debugger(&(ts->debugger), ERROR, "Select error", ok_itostr(errno), MYDEBUG);
			continue ;
		}
		// IF SELECT RETURNS 0  MAYBE CONTINUE ???
		// NEED TO CHECK THAT NO UNPROCESSED MESSAGES IN BUFFER
		else if (!ts->activity)
			continue ;
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(ts->master_socket, &readfds)) 
		{ 
			if ((ts->new_socket = accept(ts->master_socket, 
					(struct sockaddr *)&ts->address, (socklen_t*)&ts->addrlen))<0) 
			{ 
				ok_debugger(&(ts->debugger), ERROR, "Accept failed", ok_itostr(errno), MYDEBUG);
				// PROPER_EXIT
				//exit(EXIT_FAILURE);
				throw (std::runtime_error("Accept failed")); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			//Information in constructor
			ts->users.insert(std::pair<int, User*>(ts->new_socket, new User(ts->new_socket, ntohs(ts->address.sin_port), inet_ntoa(ts->address.sin_addr))));
		} 
			
		//else its some IO operation on some other socket

		//ITERATE OVER CONNECTIONS
		// READING AND SENDING LOOP
		// if (ts->state & READING_LOOP)
		// {
		for (std::map<int, User *>::iterator it = ts->users.begin(); it != ts->users.end(); /*iterating in loop*/)
		{
			std::map<int, User *>::iterator temp = it;
			ts->sd = it->first;
			User * user_ptr = it->second;
			it++;
			// if (DEEPDEBUG)
			// 	ok_debugger(&(ts->debugger), DEBUG, "Reading for loop ", ok_itostr(ts->sd), MYDEBUG);
			
			//READ FROM READING FDS
			if (FD_ISSET(ts->sd , &readfds)) 
			{
				// ok_debugger(&(ts->debugger), DEBUG, "SD is set: ", ok_itostr(ts->sd), MYDEBUG);
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((ts->valread = recv(ts->sd , ts->buffer, BUFFER_SIZE, MSG_NOSIGNAL)) <= 0) 
				{ 
					//Somebody disconnected , get his details and print 
					// getpeername(ts->sd, (struct sockaddr*)&ts->address, (socklen_t*)&ts->addrlen); 
					//Close the socket and mark as 0 in list for reuse
					remove_user_from_server(ts, user_ptr, "Quit: Connection closed by client");
					ok_debugger(&(ts->debugger), DEBUG, "Closing connection on sd (failed recv): ", ok_itostr(ts->sd), MYDEBUG);
					close(ts->sd);
					//REMOVE CONNECTION FROM MAP
					if (user_ptr->getNick().size())
						ts->nicks.erase(ts->nicks.find(user_ptr->getNick()));
					delete user_ptr;
					ts->users.erase(temp);
				} 
				//Print the message that came in 
				else
				{
					std::string	buff;
					for (int i = 0; i < ts->valread; i++)
					{
						user_ptr->getDataIn().push_back(ts->buffer[i]);
						buff.push_back(ts->buffer[i]);
					}
					// ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(ts->sd) + "] Buffer:", ok_display_buffer(&(ts->debugger), buff), MYDEBUG);
					ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(ts->sd) + "] Buffer:", ok_display_real_buffer(&(ts->debugger), user_ptr->getDataIn()), MYDEBUG);
					//while CRLF in data
					// get position of the crlf
					// if data overflow flag
					//   remove data till CRLF and remove the flag
					// copy the data till CRLF to string
					// remove data till CRLF from the data
					// try to extract the message from the string
					// if valid message
					//   add it to multimap
					// els
					//   increase error message counter

					size_t	pos;
					while ((pos = ok_crlf_finder(user_ptr->getDataIn())))
					{
						if (user_ptr->getOverflowFlag())
						{
							user_ptr->getDataIn().erase(user_ptr->getDataIn().begin(), user_ptr->getDataIn().begin() + pos);
							user_ptr->unsetOverflowFlag();
							continue ;
						}
						std::string msg;
						// pos - 2 because CRLF is not needed in the string, it was verified here
						msg.assign(user_ptr->getDataIn().begin(), user_ptr->getDataIn().begin() + pos - 2);
						if (DEEPDEBUG)
						{
							ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(ts->sd) + "] Message extracted:", msg, MYDEBUG);
						}
						user_ptr->getDataIn().erase(user_ptr->getDataIn().begin(), user_ptr->getDataIn().begin() + pos);
						// try construct message
						// if success
						//   add to multimap
						// catch exception and throw away
						try
						{
							ts->messages.insert(std::pair<int, Message*>(ts->sd, new Message(ts->sd, msg)));
							// ts->state = SENDING_LOOP;
							user_ptr->unsetReadingFlag();
						}
						catch(const std::exception& e)
						{
							ok_debugger(&(ts->debugger), ERROR, "Message: " + msg + " is not valid, because: ", e.what(), MYDEBUG);
						}
					}


					//if data >BUFFER_SIZE without CRLF
					// clear the data from data
					// set the  data overflow flag
					// if it ends with \r
					//   keep \r in the data

					if (user_ptr->getDataIn().size() > BUFFER_SIZE)
					{
						if (DEEPDEBUG)
							ok_debugger(&(ts->debugger), ERROR, "[" + ok_itostr(user_ptr->getSD()) + "]Data is overflowing", "", MYDEBUG);
						user_ptr->setOverflowFlag();
						if (*(user_ptr->getDataIn().rbegin()) == '\r')
						{
							user_ptr->getDataIn().clear();
							user_ptr->getDataIn().push_back('\r');
						}
						else
						{
							user_ptr->getDataIn().clear();
						}
					}
				}
			}
			else if (FD_ISSET(ts->sd , &writefds))
			{
				//send replies
				std::string	outbuff(user_ptr->getDataOut().begin(), user_ptr->getDataOut().end());
				ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(ts->sd) + "] Sending:", ok_display_send_buffer(ts->debugger.colour, user_ptr->getDataOut()), MYDEBUG);
				if ((ts->valsent = send(ts->sd , outbuff.c_str(), outbuff.size(), MSG_NOSIGNAL)) <= 0) 
				{ 
					//Somebody disconnected , get his details and print 
					// getpeername(ts->sd, (struct sockaddr*)&ts->address, (socklen_t*)&ts->addrlen); 
					//Close the socket and mark as 0 in list for reuse
					remove_user_from_server(ts, user_ptr, "Quit: Connection closed by client");
					ok_debugger(&(ts->debugger), DEBUG, "Closing connection on sd (failed send): ", ok_itostr(ts->sd), MYDEBUG);
					close(ts->sd);
					//REMOVE CONNECTION FROM MAP
					if (user_ptr->getNick().size())
						ts->nicks.erase(ts->nicks.find(user_ptr->getNick()));
					delete user_ptr;
					ts->users.erase(temp);
				} 
				//Print the message that came in 
				else
				{
					user_ptr->getDataOut().erase(user_ptr->getDataOut().begin(), user_ptr->getDataOut().begin() + ts->valsent);
					//deque would be better for erasing front chunks
					//ok_debugger(&(ts->debugger), DEBUG, "After sending:", "[" + ok_itostr(ts->sd) + "]" + ok_display_real_buffer(&(ts->debugger), user_ptr->getDataOut()), MYDEBUG);
				}
				if (ts->users[ts->sd]->getFreedom() && !user_ptr->getDataOut().size())
				{ 
					ok_debugger(&(ts->debugger), DEBUG, "Freeing connection on sd: ", ok_itostr(ts->sd), MYDEBUG);
					close(ts->sd);
					//REMOVE CONNECTION FROM MAP
					if (user_ptr->getNick().size())
						ts->nicks.erase(ts->nicks.find(user_ptr->getNick()));
					delete user_ptr;
					ts->users.erase(temp);
				} 
			}
			else
			{
				//fd is not ready
				//check for pending messages and timeout???
			}
		}
		//iterate over messages
		{
			// SEND TO WRITING FDS
			// SEND WITH YELLOW COLOUR
				// for (std::multimap<int, Message*>::iterator it = ts->messages.begin(); it != ts->messages.end(); /*iterating in the loop */)
				// {
			std::multimap<int, Message*>::iterator iter;
			std::multimap<int, Message*>::iterator temp_it;
			iter = ts->messages.begin();
			while (iter != ts->messages.end())
			{
				Message * msg_ptr = iter->second;

				// find if the command is in commands
				//	execute
				if (ts->commands.find(msg_ptr->getCommand()) != ts->commands.end())
				{
					ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(msg_ptr->getSD()) + "] Executing command:", ok_display_message(&(ts->debugger), msg_ptr->getMessage()), MYDEBUG);
					//maybe run in try and catch block
					ts->commands[msg_ptr->getCommand()](msg_ptr, ts);
				}
				else
				{
					err_unknowncommand_421(ts, msg_ptr->getSD(), msg_ptr->getCommand());
					//strike count of invalid messages
				}
				delete msg_ptr;
				temp_it = iter++;
				ts->messages.erase(temp_it);
			}
		}
		//print debugs
		if (FD_ISSET(ts->debugger.fd , &writefds))
		{
			ssize_t wb;//number of written bytes
			wb = write(ts->debugger.fd, ts->debugger.log.c_str(), ts->debugger.log.size());
			if (wb < 0)
			{
				//error ignored for now
			}
			else if ((size_t)wb <= ts->debugger.log.size())
			{
				ts->debugger.log.erase(0,wb);
			}
			else
			{
				//?????
			}
		}
	}
}



//clean server
void	clean_server(t_server *ts)
{
	if (DEEPDEBUG)
		ok_debugger(&(ts->debugger), WARNING, "Main loop terminating...", "", MYDEBUG);
	// ITERATE OVER MAP AND DELETE EVERYTHING
	for (std::map<int, User *>::iterator it = ts->users.begin(); it != ts->users.end(); /*iterating in the loop*/)
	{
		if (DEEPDEBUG)
		{
			ok_debugger(&(ts->debugger), INFO, "Removing connection on sd: " + ok_itostr(it->first) + ". IP: " + it->second->getIP() + ", port: " + ok_itostr(it->second->getPort()), "", MYDEBUG);
		}
		delete it->second;
		std::map<int, User *>::iterator temp = it;
		it++;
		ts->users.erase(temp);
	}
	for (std::multimap<int, Message*>::iterator it = ts->messages.begin(); it != ts->messages.end(); /*iterating in the loop*/)
	{
		if (DEEPDEBUG)
		{
			ok_debugger(&(ts->debugger), INFO, "Removing message on sd: " + ok_itostr(it->first), "", MYDEBUG);
		}
		delete it->second;
		std::multimap<int, Message*>::iterator temp = it;
		it++;
		ts->messages.erase(temp);
	}
	for (std::map<std::string, Channel *>::iterator it = ts->channels.begin(); it != ts->channels.end(); /*iterating in the loop*/)
	{
		if (DEEPDEBUG)
		{
			ok_debugger(&(ts->debugger), INFO, "Removing channel: " + it->first, "", MYDEBUG);
		}
		delete it->second;
		std::map<std::string, Channel *>::iterator temp = it;
		it++;
		ts->channels.erase(temp);
	}
	ssize_t	wb;
	wb = write(ts->debugger.fd, ts->debugger.log.c_str(), ts->debugger.log.size());
	if (wb < 0)
	{
		std::cerr << "failed write" << std::endl;
	}
	else if ((size_t)wb <= ts->debugger.log.size())
	{
		ts->debugger.log.erase(0,wb);
	}
	//erase multimaps?
}

// create map of commands and pointers to functions to process them
void	init_server(t_server *ts)
{
	ts->opt = TRUE;
	//create a master socket 
	if( (ts->master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{
		ok_debugger(&(ts->debugger), ERROR, "Socket failed", ok_itostr(errno), MYDEBUG);
		//exit(EXIT_FAILURE);
		throw (std::runtime_error("Socket failed")); 
	} 
	
	//server starts by listening for new users
	ts->state = READING_LOOP;

	//wait for an activity on one of the sockets , timeout is not NULL
	// so it is non blocking
	//timeout set for 15 s 0 ms
	ts->timeout.tv_sec = 15;
	ts->timeout.tv_usec = 0;

	//set master socket to allow multiple users , 
	//this is just a good habit, it will work without this 
	if( setsockopt(ts->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&ts->opt, 
		sizeof(ts->opt)) < 0 ) 
	{ 
		ok_debugger(&(ts->debugger), ERROR, "Setsockopt failed", ok_itostr(errno), MYDEBUG);
		//exit(EXIT_FAILURE);
		throw (std::runtime_error("Setsockopt failed")); 
	} 
	
	//type of socket created 
	ts->address.sin_family = AF_INET; 
	ts->address.sin_addr.s_addr = INADDR_ANY; //is it localhost or any IP address on the machine?
	ts->address.sin_port = htons(ts->port); 
		
	//bind the socket to localhost port 8888 
	if (bind(ts->master_socket, (struct sockaddr *)&ts->address, sizeof(ts->address))<0) 
	{ 
		ok_debugger(&(ts->debugger), ERROR, "Bind failed", ok_itostr(errno), MYDEBUG);
		//exit(EXIT_FAILURE);
		throw (std::runtime_error("Bind failed")); 
	}
	ok_debugger(&(ts->debugger), INFO, std::string("Listening on port: ") + ok_itostr(ts->port), "", MYDEBUG);
	//try to specify maximum of 3 pending users for the master socket 
	if (listen(ts->master_socket, 3) < 0) 
	{ 
		ok_debugger(&(ts->debugger), ERROR, "Listen failed", ok_itostr(errno), MYDEBUG);
		//exit(EXIT_FAILURE);
		throw (std::runtime_error("Listen failed")); 
	} 
		
	//accept the incoming connection 
	ts->addrlen = sizeof(ts->address); 
	ok_debugger(&(ts->debugger), INFO, "Waiting for users ...", "", MYDEBUG);

	//add commands
	ts->commands["CAP"] = irc_cap;
	ts->commands["PASS"] = irc_pass;
	ts->commands["NICK"] = irc_nick;
	ts->commands["USER"] = irc_user;
	ts->commands["PING"] = irc_ping;
	ts->commands["PONG"] = irc_pong;
	ts->commands["MODE"] = irc_mode;
	ts->commands["JOIN"] = irc_join;
	ts->commands["PART"] = irc_part;
	ts->commands["PRIVMSG"] = irc_privmsg;
	ts->commands["NOTICE"] = irc_notice;
	ts->commands["TOPIC"] = irc_topic;
	ts->commands["INVITE"] = irc_invite;
	ts->commands["KICK"] = irc_kick;
	ts->commands["AWAY"] = irc_away;
	ts->commands["WHO"] = irc_who;
	ts->commands["WHOIS"] = irc_whois; //debug
	ts->commands["QUIT"] = irc_quit;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;
	// ts->commands[""] = irc_;

	// ok_debugger(&(ts->debugger), DEBUG, "This is a debug message", "", MYDEBUG);
	// ok_debugger(&(ts->debugger), INFO, "This is an info message", "", MYDEBUG);
	// ok_debugger(&(ts->debugger), NOTICE, "This is a notice message", "", MYDEBUG);
	// ok_debugger(&(ts->debugger), WARNING, "This is a warning message", "", MYDEBUG);
	// ok_debugger(&(ts->debugger), ERROR, "This is an error message", "", MYDEBUG);
	//main server loop
	server_loop(ts);
}



static bool ok_argvcheck(char *argv, t_server *ts)
{
	ts->debugger.debuglvl = WARNING;
	if (DEEPDEBUG)
		ts->debugger.debuglvl = DEBUG;
	if (!argv)
		return (true);
	std::string	str(argv);
	if (str == "--debug-lvl=0")
	{
		ts->debugger.debuglvl = EXTRADEBUG;
		return (true);
	}
	if (str == "--debug-lvl=1")
	{
		ts->debugger.debuglvl = DEBUG;
		return (true);
	}
	if (str == "--debug-lvl=2")
	{
		ts->debugger.debuglvl = INFO;
		return (true);
	}
	if (str == "--debug-lvl=3")
	{
		ts->debugger.debuglvl = NOTICE;
		return (true);
	}
	if (str == "--debug-lvl=4")
	{
		ts->debugger.debuglvl = WARNING;
		return (true);
	}
	if (str == "--debug-lvl=5")
	{
		ts->debugger.debuglvl = ERROR;
		return (true);
	}
	if (str == "--debug-lvl=6")
	{
		ts->debugger.debuglvl = DISABLED;
		return (true);
	}
	return (false);
}

//debug levels
// 0 debug
// 1 info
// 2 notice
// 3 warnings (default)
// 4 errors only


// typedef struct s_debugger {
// 	bool		date;
// 	bool		time;
// 	bool		utime;
// 	bool		colour;
// 	bool		extra;
// 	int			fd;
// 	DebugLvl	debuglvl;
// }	t_debugger;

static void ft_init_debugger(s_debugger *debugger)
{
	debugger->date = false;
	debugger->time = true;
	debugger->utime = true;
	debugger->precision = 4; // should be 0-6
	debugger->colour = true;
	debugger->extra = true;
	debugger->fd = 1;
}

// PORT=5555;
// #IP=10.12.1.3
// PASSWORD=abc;
// DEBUG_LVL=debug;			#debug, info, notice, warning, error, disabled
// DEBUG_FD=2; 
// #DEBUGFILE="debug.txt"
// DEBUG_DATE=0;			# show date
// DEBUG_TIME=1;			# show time
// DEBUG_UTIME=1;			# show microseconds
// DEBUG_UPRECISION=4;		# precision of time in microseconds
// DEBUG_COLOUR=1;			# use ansi escape codes
// DEBUG_EXTRA=1;			# show file:function:line

static int ft_read_config(t_server *ts)
{
	std::ifstream	file(".ft_irc.conf");
	std::string		line;
	std::string		key;
	std::string		value;

	ts->servername = "bestirc";
	if (file.fail() || !file.is_open())
		return (1);
	while (std::getline(file, line))
	{
		size_t index = line.find('=');
		if (index == std::string::npos || index == line.size())
			continue ;
		key = line.substr(0, index);
		size_t end = line.find(';');
		if (line.size() && line[0] == '#')
			continue ;
		if (end == std::string::npos)
		{
			continue ;
		}
		value = line.substr(index + 1, end - index -1); //+1 to skip '=' and -1 to ignore ';'
		if (key == "PORT")
		{
			if (ok_strtoi<int>(value) < 0 || ok_strtoi<int>(value) > 65535)
				return (1);
			ts->port = ok_strtoi<int>(value);
		}
		else if (key == "IP")
		{
			//not implemented
		}
		else if (key == "SERVERNAME")
		{
			ts->servername = value;
		}
		else if (key == "PASSWORD")
		{
			ts->password = value;
		}
		else if (key == "DEBUG_LVL")
		{
			if (value == "debug")
				ts->debugger.debuglvl = DEBUG;
			else if (value == "info")
				ts->debugger.debuglvl = INFO;
			else if (value == "notice")
				ts->debugger.debuglvl = NOTICE;
			else if (value == "warning")
				ts->debugger.debuglvl = WARNING;
			else if (value == "error")
				ts->debugger.debuglvl = ERROR;
			else if (value == "disabled")
				ts->debugger.debuglvl = DISABLED;
			else
				std::cerr << "Invalid DEBUG_LVL: " << value << std::endl;
		}
		else if (key == "DEBUG_FD")
		{
			if (value == "1")
				ts->debugger.fd = 1;
			else if (value == "2")
				ts->debugger.fd = 2;
			else
				std::cerr << "Invalid DEBUG_FD: " << value << std::endl;
		}
		else if (key == "DEBUG_DATE")
		{
			if (value == "0")
				ts->debugger.date = false;
			else if (value == "1")
				ts->debugger.date = true;
			else
				std::cerr << "Invalid DEBUG_DATE: " << value << std::endl;
		}
		else if (key == "DEBUG_TIME")
		{
			if (value == "0")
				ts->debugger.time = false;
			else if (value == "1")
				ts->debugger.time = true;
			else
				std::cerr << "Invalid DEBUG_TIME: " << value << std::endl;
		}
		else if (key == "DEBUG_UTIME")
		{
			if (value == "0")
				ts->debugger.utime = false;
			else if (value == "1")
				ts->debugger.utime = true;
			else
				std::cerr << "Invalid DEBUG_UTIME: " << value << std::endl;
		}
		else if (key == "DEBUG_UPRECISION")
		{
			if (value == "0")
				ts->debugger.precision = 0;
			else if (value == "1")
				ts->debugger.precision = 1;
			else if (value == "2")
				ts->debugger.precision = 2;
			else if (value == "3")
				ts->debugger.precision = 3;
			else if (value == "4")
				ts->debugger.precision = 4;
			else if (value == "5")
				ts->debugger.precision = 5;
			else if (value == "6")
				ts->debugger.precision = 6;
			else
				std::cerr << "Invalid DEBUG_UPRECISION: " << value << std::endl;
		}
		else if (key == "DEBUG_COLOUR")
		{
			if (value == "0")
				ts->debugger.colour = false;
			else if (value == "1")
				ts->debugger.colour = true;
			else
				std::cerr << "Invalid DEBUG_COLOUR: " << value << std::endl;
		}
		else if (key == "DEBUG_EXTRA")
		{
			if (value == "0")
				ts->debugger.extra = false;
			else if (value == "1")
				ts->debugger.extra = true;
			else
				std::cerr << "Invalid DEBUG_EXTRA: " << value << std::endl;
		}
		else
				std::cerr << "Unknown key: " << key << std::endl;
	}
	if (!ts->port)
		return (1);
	if (ts->password.size() == 0)
		return (1);
	return (0);
}

int	main(int argc , char *argv[]) 
{
	t_server	ts;
	//check argv1
	//check argv2?
	ts.port = 0;
	signal(SIGINT, signal_handler); 
	if (argc < 3 || argc > 4)
	{
		ft_init_debugger(&(ts.debugger));
		if (ft_read_config(&ts))
			return (ft_usage());
	}
	else
	{
		ts.port = ok_strtoi<int>(argv[1]);
		if (ts.port <=0 || ts.port > 65535)
			return (ft_usage_port());
		if (!ok_argvcheck(argv[3], &ts))
			return (ft_usage_debug());
		ft_init_debugger(&(ts.debugger));
		ts.password = argv[2];
	}
	ok_debugger(&(ts.debugger), NOTICE, std::string("DEBUG LEVEL: ") + ok_itostr(ts.debugger.debuglvl), "", MYDEBUG);
	try
	{
		init_server(&ts);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		clean_server(&ts);
		return (1);
	}
	clean_server(&ts);
	//try init server
	// catch
	//	return 1;
	return 0; 
} 
