//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket users with select and fd_set on Linux 

#include "server.hpp"

static bool		g_server_alive = true;

void signal_handler(int signal_num) 
{
	if (signal_num == SIGINT)
		g_server_alive = false;
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

static bool isChannel(struct s_server *ts, std::string channelName)
{
	return (ts->channels.find(channelName) != ts->channels.end());
}

static bool isInChannel(struct s_server *ts, std::string channelName, std::string nick)
{
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		if (it->second->getNick() == nick)
			return (true);
	}
	return (false);
}

static int	remove_user_from_channel(struct s_server *ts, User *user, Channel *channel)
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
			ok_debugger(&(ts->debugger), NOTICE, "Deleting channel for user: ", channelName, MYDEBUG);
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

static int	remove_user_from_server(struct s_server *ts, User *user)
{
	for (std::multimap<std::string, Channel*>::iterator it = ts->user2channel.lower_bound(user->getNick()); it != ts->user2channel.upper_bound(user->getNick());)
	{
		std::multimap<std::string, Channel*>::iterator temp = it++;
		remove_user_from_channel(ts, user, temp->second);
	}
	//user freedom = true;
	return (0);
}

static int	add_user_to_channel(struct s_server *ts, User *user, Channel *channel)
{
	//add channel to multimap
	ts->user2channel.insert(std::pair<std::string, Channel*>(user->getNick(), channel));
	//add user to multimap
	ts->channel2user.insert(std::pair<std::string, User*>(channel->getChannelName(), user));
	if (channel->addUser(user->getSD()) == 1)
		channel->addOperator(user->getSD());
	return (0);
}

static void	send_reply(struct s_server *ts, unsigned short sd, User *Sender, std::string text)
{
	std::string	reply;

	if (Sender)
		reply += ":" + Sender->getNick() + "!" + Sender->getUserName() + "@" + Sender->getIP() + " ";
	reply += text + CRLF;
	ok_debugger(&(ts->debugger), DEBUG, "[" + ok_itostr(sd) + "]", ok_display_reply(&(ts->debugger), reply), MYDEBUG);
	for (std::string::iterator it = reply.begin(); it != reply.end(); it++)
	{
		ts->users[sd]->_data_out.push_back(*it);
	}
}

static void	send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text)
{
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		send_reply(ts, it->second->getSD(), Sender, text);
	}
}

// static void	send_reply_others(struct s_server *ts, std::string channelName, User *Sender, std::string text)
// {
// 	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
// 	{
// 		if (it->second->getNick() != ts->users[Sender->getSD()]->getNick())
// 		{
// 			send_reply(ts, it->second->getSD(), Sender, text);
// 		}
// 	}
// }

// CAP LS
void irc_cap(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "CAP COMMAND not supported" NO_COLOUR << std::endl; 
	(void)msg;
	(void)ts;
}

// PASS a
// https://modern.ircdocs.horse/#pass-message
// Numeric replies:
// 



void irc_pass(Message* msg, struct s_server *ts)
{
	std::string reply;
	std::cout << MAGENTA_COLOUR "PASS COMMAND not fully supported" NO_COLOUR << std::endl; 

	if (!msg->getParams().size())
	{
		reply = "461 ";
		reply += ts->users[msg->getSD()]->getIP() + " ";
		reply += "PASS :Not enough parameters";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	// ERR_ALREADYREGISTERED (462)
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		reply = "462 ";
		if (!ts->users[msg->getSD()]->getNick().size())
			reply += ts->users[msg->getSD()]->getNick() + " ";
		else
			reply += ts->users[msg->getSD()]->getIP() + " ";
		reply += ":You may not reregister";
		send_reply(ts, msg->getSD(), NULL, reply);
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

# define LOWER "abcdefghijklmnopqrstuvwxyz"
# define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define DIGITS "0123456789"
# define SPECIAL "[]{}^`|_\\"

static bool	isValidNick(std::string	nick)
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

	std::cout << MAGENTA_COLOUR "NICK COMMAND is now fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlags() & PASSWORD))
		return ;
	// std::cout << MAGENTA_COLOUR "User " << msg->getSD()
	// << " setting nick " << newnick << NO_COLOUR << std::endl;
	if (ts->users[msg->getSD()]->getNick().size())
		oldnick = ts->users[msg->getSD()]->getNick();
	//check if parameter
	if (!msg->getParams().size())
	{
		reply = "431 ";
		if (oldnick.size())
			reply += oldnick + " ";
		else
			reply += ts->users[msg->getSD()]->getIP() + " ";
		reply += ":No nickname given";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	std::string	newnick = msg->getParams()[0];
	if (!isValidNick(newnick))
	{
		reply = "432 ";
		if (oldnick.size())
			reply += oldnick + " ";
		else
			reply += ts->users[msg->getSD()]->getIP() + " ";
		reply += newnick + " :Erroneus nickname";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	if (ts->nicks.find(newnick) != ts->nicks.end())
	{
		reply = "433 ";
		if (oldnick.size())
			reply += oldnick + " ";
		else
			reply += ts->users[msg->getSD()]->getIP() + " ";
		reply += newnick + " :Nickname is already in use";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	//setting the first (and only?) parameter as a nick in the map of users
	//accessed by the socket descriptor gained from the message class

	ts->users[msg->getSD()]->setNick(newnick);
	if (oldnick.size())
	{
		ts->nicks.erase(ts->nicks.find(oldnick));
	}
	
	ts->nicks.insert(std::pair<std::string, User*>(newnick, ts->users[msg->getSD()]));
	// ts.users.insert(std::pair<int, User*>(ts.new_socket, new User(ts.new_socket, ntohs(ts.address.sin_port), inet_ntoa(ts.address.sin_addr))));
	if (ts->users[msg->getSD()]->getNick().size())
	{
		//send to all?
		//:net!net@127.0.0.1 << need to be not hardcoded
		reply = ":" + oldnick + "!" + ts->users[msg->getSD()]->getUserName() + "@" + ts->users[msg->getSD()]->getIP();
		reply += " NICK :" +  ts->users[msg->getSD()]->getNick();
		send_reply(ts, msg->getSD(), NULL, reply);
		// if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
		// { 
		// 	std::cerr << "send failed" << std::endl;
		// } 
		// else
		// {
		// 	std::cout << "Reply message sent successfully" << std::endl;
		// }
	}
	if (!oldnick.size())
	{
		ts->users[msg->getSD()]->addAuthFlag(NICK);
		if (ts->users[msg->getSD()]->getAuthFlag())
		{
			reply = "001 " +  ts->users[msg->getSD()]->getNick() + " :Hello there";
			send_reply(ts, msg->getSD(), NULL, reply);
		}
	}

	std::cout << MAGENTA_COLOUR "User " << msg->getSD()
	<< " got nick " << ts->users[msg->getSD()]->getNick() << NO_COLOUR << std::endl;
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
	std::string	reply;
	std::cout << MAGENTA_COLOUR "USER COMMAND is now fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlags() & PASSWORD))
		return ;
	// we need to check that there are 4 parameters?
	if (msg->getParams().size() != 4 || !msg->getParams()[0].size())
	{
		reply = "461 ";
		if (!ts->users[msg->getSD()]->getNick().size())
			reply += ts->users[msg->getSD()]->getNick() + " ";
		else
			reply += ts->users[msg->getSD()]->getIP() + " ";
		reply += "USER :Not enough parameters";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		reply = "462 ";
		reply += ts->users[msg->getSD()]->getNick();
		reply += " :You may not register again";
		send_reply(ts, msg->getSD(), NULL, reply);
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
		reply = "001 " +  ts->users[msg->getSD()]->getNick() + " :Hello there";
		send_reply(ts, msg->getSD(), NULL, reply);
	}
	// reply = "001 " +  ts->users[msg->getSD()]->getNick() + " :Hello there";
	// send_reply(ts, msg->getSD(), NULL, reply);
	// // if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
	// { 
	// 	std::cerr << "send failed" << std::endl;
	// } 
	// else
	// {
	// 	std::cout << "Reply message sent successfully" << std::endl;
	// }
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

	std::cout << MAGENTA_COLOUR "PING COMMAND is almost supported" NO_COLOUR << std::endl; 
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	reply = "PONG " +  ts->users[msg->getSD()]->getNick() + " :" + msg->getParams()[0];
	send_reply(ts, msg->getSD(), NULL, reply);
	// if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
	// { 
	// 	std::cerr << "send failed" << std::endl;
	// } 
	// else
	// {
	// 	std::cout << "Reply message sent successfully" << std::endl;
	// }
}

// PONG
// https://modern.ircdocs.horse/#pong-message
void irc_pong(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "PONG COMMAND not supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
}

// QUIT
// :dan-!d@localhost QUIT :Quit: Bye for now!
// https://modern.ircdocs.horse/#quit-message
void irc_quit(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "QUIT COMMAND not yet supported" NO_COLOUR << std::endl; 
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	remove_user_from_server(ts, ts->users[msg->getSD()]);
	std::string	reply;
	reply = "ERROR :Quitting channel";
	send_reply(ts, msg->getSD(), NULL, reply);
	//user leave all the channels they are in
	//send quitting message to all uers that share the channels
}


static std::vector<std::string>	ok_split(std::string str, char c)
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


// Error
// https://modern.ircdocs.horse/#error-message

/* CHANNEL OPERATIONS */

// JOIN
// https://modern.ircdocs.horse/#join-message
void irc_join(Message* msg, struct s_server *ts)
{
	std::string	reply;
	std::cout << MAGENTA_COLOUR "JOIN COMMAND now fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		reply = "461 ";
		reply += ts->users[msg->getSD()]->getNick() + " ";
		reply += "JOIN :Not enough parameters";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	std::vector<std::string>	channelNames = ok_split(msg->getParams()[0], ',');
	std::vector<std::string>	keys;
	if (msg->getParams().size() > 1)
		keys =  ok_split(msg->getParams()[1], ',');
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		//check if the channel name valid
		if (channelNames[i].size() < 2 || (channelNames[i][0] != '#' && channelNames[i][0] != '@'))
		{
			reply = "476 ";
			reply += channelNames[i] + " ";
			reply += ":Bad Channel Mask";
			send_reply(ts, msg->getSD(), NULL, reply);
			break ;
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
				std::cout << REDBG_COLOUR "Channel " NO_COLOUR << std::endl;
				std::cout << RED_COLOUR << channelNames[i] << NO_COLOUR << std::endl;
				std::cout << REDBG_COLOUR " could not be created, because: " NO_COLOUR << std::endl;
				std::cout << RED_COLOUR << e.what() << NO_COLOUR << std::endl;
				return ;
			}
		}
		else
		{
			//check if user can join the channel
			if (ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_INVITE)
			{
				reply = "473 ";
				reply += ts->users[msg->getSD()]->getNick() + " ";
				reply += channelNames[i] + " ";
				reply += ":Cannot join channel (+i)";
				send_reply(ts, msg->getSD(), NULL, reply);
				break ;
			}
			if ((ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_LIMIT) && ts->channels[channelNames[i]]->getUsers() >= ts->channels[channelNames[i]]->getChannelLimit())
			{
				reply = "471 ";
				reply += ts->users[msg->getSD()]->getNick() + " ";
				reply += channelNames[i] + " ";
				reply += ":Cannot join channel (+l)";
				send_reply(ts, msg->getSD(), NULL, reply);
				break ;
			}
			if ((ts->channels[channelNames[i]]->getModeFlags() & CHANNEL_KEY))
			{
				if (keys.size() <= i || keys[i] != ts->channels[channelNames[i]]->getKey())
				{
					reply = "475 ";
					reply += ts->users[msg->getSD()]->getNick() + " ";
					reply += channelNames[i] + " ";
					reply += ":Cannot join channel (+k)";
					send_reply(ts, msg->getSD(), NULL, reply);
					break ;
				}
			}
			ok_debugger(&(ts->debugger), DEBUG, "This channel already exists: ", channelNames[i], MYDEBUG);
		}
			
		add_user_to_channel(ts, ts->users[msg->getSD()], ts->channels[channelNames[i]]);
		//send everyone join message
		reply = "JOIN :" + channelNames[i];
		send_reply_channel(ts, channelNames[i], ts->users[msg->getSD()], reply);
		//332
		reply = "332 ";
		reply += ts->users[msg->getSD()]->getNick() + " ";
		reply += channelNames[i] + " ";
		reply += ":" + ts->channels[channelNames[i]]->getTopic();
		send_reply(ts, msg->getSD(), NULL, reply);
		//353
		reply = "353 ";
		reply += ts->users[msg->getSD()]->getNick() + " ";
		reply += channelNames[i] + " ";
		reply += ":";
		for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelNames[i]); it != ts->channel2user.upper_bound(channelNames[i]); it++)
		{
			if (ts->channels[channelNames[i]]->isOperator(it->second->getSD()))
				reply += "@";
			reply += it->second->getNick() + " ";
		}
		reply.resize(reply.size() - 1);
;		send_reply(ts, msg->getSD(), NULL, reply);
		//353
		
		// for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelNames[i]); it != ts->channel2user.upper_bound(channelNames[i]); it++)
		// {
		// 	reply = "353 ";
		// 	reply += ts->users[msg->getSD()]->getNick() + " ";
		// 	reply += channelNames[i] + " ";
		// 	reply += ":";
		// 	if (ts->channels[channelNames[i]]->isOperator(it->second->getSD()))
		// 		reply += "@";
		// 	reply += it->second->getNick() + " ";
		// 	send_reply(ts, msg->getSD(), NULL, reply);
		// }
		// 366
		reply = "366 ";
		reply += ts->users[msg->getSD()]->getNick() + " ";
		reply += channelNames[i] + " ";
		reply += ":End of /NAMES list";
		send_reply(ts, msg->getSD(), NULL, reply);
	}
}

// PART
//https://modern.ircdocs.horse/#part-message
void irc_part(Message* msg, struct s_server *ts)
{
	std::string	reply;
	std::cout << MAGENTA_COLOUR "PART COMMAND not fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		reply = "461 ";
		reply += ts->users[msg->getSD()]->getNick() + " ";
		reply += "PART :Not enough parameters";
		send_reply(ts, msg->getSD(), NULL, reply);
		return ;
	}
	std::string	channelName = msg->getParams()[0];
	//check if the channel name valid
	//check if user can join the channel

	//if the channel does not exist
	if (ts->channels.find(channelName) == ts->channels.end())
	{
		ok_debugger(&(ts->debugger), WARNING, "Cannot leave non-existing channel: ", channelName, MYDEBUG);
		return ;
	}
	else
	{
		ok_debugger(&(ts->debugger), DEBUG, "Leaving channel: ", channelName, MYDEBUG);
		reply = ":" + ts->users[msg->getSD()]->getNick() + "!" + ts->users[msg->getSD()]->getUserName() + "@" + ts->users[msg->getSD()]->getIP();
		reply += " PART " + channelName + " :" + (msg->getParams().size() > 1 ? msg->getParams()[1] : std::string("leaving"));
		send_reply(ts, msg->getSD(), NULL, reply);
		// if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
		// { 
		// 	std::cerr << "send failed" << std::endl;
		// } 
		// else
		// {
		// 	std::cout << "Reply message sent successfully" << std::endl;
		// }
	}
	remove_user_from_channel(ts, ts->users[msg->getSD()], ts->channels[channelName]);
}

//TOPIC
// https://modern.ircdocs.horse/#topic-message
//TOPIC #test :Hello
//:user123!net@127.0.0.1 TOPIC #test :Hello

void irc_topic(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "TOPIC COMMAND not fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	//send_reply_channel(struct s_server *ts, std::string channelName, User *Sender, std::string text, int flags)
	if (msg->getParams().size() < 2)
	{
		std::cerr << "NOT enough parameters" << std::endl;
		//add reply error
		return ;
	}
	std::string channelName = msg->getParams()[0];
	std::string topic = msg->getParams()[1];
	std::string text = "TOPIC " + channelName + " :" + topic;
	if (ts->channels.find(channelName) != ts->channels.end())
	{
		ts->channels[channelName]->setTopic(topic);
		send_reply_channel(ts, channelName, ts->users[msg->getSD()], text);
	}
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
	std::cout << MAGENTA_COLOUR "INVITE COMMAND not fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	// if user exists and if user not in channel, join user to channel
	if (msg->getParams().size() < 2)
	{
		std::cerr << "NOT enough parameters" << std::endl;
		//add reply error
		return ;
	}
	std::string nick = msg->getParams()[0];
	std::string channelName = msg->getParams()[1];

	if (ts->nicks.find(nick) == ts->nicks.end() || ts->channels.find(channelName) == ts->channels.end())
	{
		std::cerr << "NICK or CHannel does not exist" << std::endl;
		return ;
	}
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		if (it->second->getNick() == nick)
		{
			std::cerr << "User already in the channel" << std::endl;
			return ;
		}
		else
			std::cerr << "User " << it->second->getNick() << " in the channel" << std::endl;
	}
	std::string	reply;
	reply = "341 INVITE " + ts->users[msg->getSD()]->getNick() + " " + nick + " " + channelName;
	send_reply(ts, msg->getSD(), NULL, reply);
	reply = "JOIN :" + channelName;
	send_reply_channel(ts, channelName, ts->nicks[nick], reply);
	send_reply(ts, ts->nicks[nick]->getSD(), ts->nicks[nick], reply);
	// //add channel to multimap
	// ts->user2channel.insert(std::pair<std::string, Channel*>(nick, ts->channels[channelName]));
	// //add user to multimap
	// ts->channel2user.insert(std::pair<std::string, User*>(channelName, ts->nicks[nick]));
	add_user_to_channel(ts, ts->nicks[nick], ts->channels[channelName]);
}

//KICK
//https://modern.ircdocs.horse/#kick-message
//  KICK #Finnish John :Speaking English
//    :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
void irc_kick(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "KICK COMMAND not fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (msg->getParams().size() < 2)
	{
		std::cerr << "NOT enough parameters" << std::endl;
		//add reply error
		return ;
	}
	std::string channelName = msg->getParams()[0];
	std::string nick = msg->getParams()[1];
	if (ts->nicks.find(nick) == ts->nicks.end() || ts->channels.find(channelName) == ts->channels.end())
	{
		std::cerr << "NICK or CHannel does not exist" << std::endl;
		return ;
	}
	for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
	{
		if (it->second->getNick() == nick)
		{
			//inform channel
			std::string	reply;
			reply = "KICK " + channelName + " " + nick;
			if (msg->getParams().size() > 2)
				reply += " :" + msg->getParams()[2];
			send_reply_channel(ts, channelName, ts->users[msg->getSD()], reply);
			std::cerr << "Removing user from channel" << std::endl;
			//remove user
			remove_user_from_channel(ts, ts->nicks[nick], ts->channels[channelName]);
			// ts->channel2user.erase(it);
			// for (std::multimap<std::string, Channel*>::iterator iter = ts->user2channel.lower_bound(nick); iter != ts->user2channel.upper_bound(nick); iter++)
			// {
			// 	if (iter->second->getChannelName() == channelName)
			// 	{
			// 		ts->user2channel.erase(iter);
			// 		std::cerr << "Removing channel from user" << std::endl;
			// 		break ;
			// 	}
			// }
			return ;
		}
	}
	//user not in channel
}


//AWAY
//https://modern.ircdocs.horse/#away-message
void irc_away(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "AWAY COMMAND not really supported" NO_COLOUR << std::endl;
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
	std::cout << MAGENTA_COLOUR "MODE COMMAND not really supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	Channel *channel;
	if (msg->getParams().size() < 2)
	{
		std::cerr << "NOT enough parameters" << std::endl;
		return ;
	}
	std::string channelName = msg->getParams()[0];
	std::string	flags = msg->getParams()[1];
	if (!channelName.size() || flags.size() < 2 || !isChannel(ts, channelName)
		|| (flags[0] != '+' && flags[0] != '-'))
	{
		std::cerr << "NOT good parameters" << std::endl;
		return ;
	}
	bool	plus = (flags[0] == '+');
	//check channel as param[0]
	//check plus or minus as param [1][0]
	//check param[1][1]
	channel = ts->channels[channelName];
	if (flags[1] == 'i')
	{
		if (plus)
		{
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " is now invite only.", "", MYDEBUG);
			channel->addModeFlags(CHANNEL_INVITE);
		}
		else
		{
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " is no longer invite only.", "", MYDEBUG);
			channel->removeModeFlags(CHANNEL_INVITE);
		}
	}
	else if (flags[1] == 't')
	{
		if (plus)
		{
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " topic is setable by moderators only.", "", MYDEBUG);
			channel->addModeFlags(CHANNEL_TOPIC);
		}
		else
		{
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " topic is setable by anyone.", "", MYDEBUG);
			channel->removeModeFlags(CHANNEL_TOPIC);
		}
	}
	else if (flags[1] == 'k')
	{
		if (plus)
		{
			if (msg->getParams().size() < 3)
			{
				std::cerr << "NOT enough parameters" << std::endl;
				return ;
			}
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " locked by key:", msg->getParams()[2], MYDEBUG);
			channel->setKey(msg->getParams()[2]);
			channel->addModeFlags(CHANNEL_KEY);
		}
		else
		{
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " unlocked.", "", MYDEBUG);
			channel->setKey("");
			channel->removeModeFlags(CHANNEL_KEY);
		}
	}
	else if (flags[1] == 'l')
	{
		if (plus)
		{
			if (msg->getParams().size() < 3)
			{
				std::cerr << "NOT enough parameters" << std::endl;
				return ;
			}
			int	limit = ok_strtoi<int>(msg->getParams()[2]);
			if (limit < 2 || limit > 256)
			{
				std::cerr << "Invalid limit: " << limit << std::endl;
				return ;
			}
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " limited to " + ok_itostr(limit) + " users.", "", MYDEBUG);
			channel->setChannelLimit(limit);
			channel->addModeFlags(CHANNEL_LIMIT);
		}
		else
		{
			ok_debugger(&(ts->debugger), DEBUG, "Channel: " + channelName + " not limited.", "", MYDEBUG);
			channel->setChannelLimit(0);
			channel->removeModeFlags(CHANNEL_LIMIT);
		}
	}
	else if (flags[1] == 'o')
	{
		if (msg->getParams().size() < 3)
		{
			std::cerr << "NOT enough parameters" << std::endl;
			return ;
		}
		std::string nick = msg->getParams()[2];
		if (!isInChannel(ts, channelName, nick))
		{
			std::cerr << "Nick " << nick << " not in channel " << channelName << "." << std::endl;
			return ;
		}
		
		if (plus)
		{
			ok_debugger(&(ts->debugger), DEBUG, ts->users[msg->getSD()]->getNick() + " added " + nick + " as an operator in the channel: " + channelName + "." , "", MYDEBUG);
			channel->addOperator(ts->nicks[nick]->getSD());
		}
		else
		{
			ok_debugger(&(ts->debugger), DEBUG, ts->users[msg->getSD()]->getNick() + " removed " + nick + " from operators in the channel: " + channelName + "." , "", MYDEBUG);
			channel->removeOperator(ts->nicks[nick]->getSD());
		}
	}
	else
	{
		std::cerr << "NOT good parameters" << std::endl;
		return ;
	}
}

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there

void irc_privmsg(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "PRIVMSG COMMAND not fully supported" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (msg->getParams().size() < 2)
	{
		std::cerr << "NOT enough parameters" << std::endl;
		//add reply error
		return ;
	}
	//message to channel vs message to user
	if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '@')
	{
		std::string channelName = msg->getParams()[0];
		//reply channel
		//check if channel exists
		if (ts->channels.find(channelName) == ts->channels.end())
		{
			ok_debugger(&(ts->debugger), WARNING, "Cannot message non-existing channel:", channelName, MYDEBUG);
			return ;
		}
		//send reply to all other users in the channel
		for (std::multimap<std::string, User*>::iterator it = ts->channel2user.lower_bound(channelName); it != ts->channel2user.upper_bound(channelName); it++)
		{
			if (it->second->getNick() != ts->users[msg->getSD()]->getNick())
			{
				
				std::string	text;
				text = "PRIVMSG " + channelName + " :" + msg->getParams()[1];
				send_reply(ts, it->second->getSD(), ts->users[msg->getSD()], text);
			}
		}
	}
	else
	{
		//reply user
	}
}

//NOTICE
//https://modern.ircdocs.horse/#notice-message
void irc_notice(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "NOTICE COMMAND not supported yet" NO_COLOUR << std::endl; 
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
}

//WHO
// https://modern.ircdocs.horse/#who-message
void irc_who(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "WHO COMMAND not supported" NO_COLOUR << std::endl; 
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
}


//WHOIS == DEBUG
void irc_whois(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "WHOIS COMMAND is secret DEBUG" NO_COLOUR << std::endl;
	if (!(ts->users[msg->getSD()]->getAuthFlag()))
		return ;
	if (!msg->getParams().size())
	{
		std::cerr << "NOT enough parameters" << std::endl;
		//add reply error
		return ;
	}
	if (msg->getParams()[0][0] == '#' || msg->getParams()[0][0] == '@')
	{
		std::string channelName = msg->getParams()[0];
		if (ts->channels.find(channelName) != ts->channels.end())
			std::cout << ts->channels[channelName]->print(true) << std::endl;
	}
	else
	{
		std::string nick = msg->getParams()[0];
		if (ts->nicks.find(nick) != ts->nicks.end())
			std::cout << ts->nicks[nick]->print(true) << std::endl;
	}
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
		if (DEEPDEBUG)
			ok_debugger(&(ts->debugger), DEBUG, "While loop started...", "", MYDEBUG);
		//clear the socket set 
		FD_ZERO(&readfds);
		FD_ZERO(&writefds); 
		//add master socket to set 
		FD_SET(ts->master_socket, &readfds); 
		ts->max_sd = ts->master_socket; 

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
			if (connection_ptr->_data_out.size())
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
					// std::cout << "Host disconnected , ip is : " << inet_ntoa(ts->address.sin_addr)
					// 	<< " , port : " << ntohs(ts->address.sin_port) << std::endl; 
					//Close the socket and mark as 0 in list for reuse
					ok_debugger(&(ts->debugger), DEBUG, "Closing connection on sd: ", ok_itostr(ts->sd), MYDEBUG);
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
						user_ptr->_data.push_back(ts->buffer[i]);
						buff.push_back(ts->buffer[i]);
					}
					// ok_debugger(&(ts->debugger), DEBUG, "Buffer:", "[" + ok_itostr(ts->sd) + "]" + ok_display_buffer(&(ts->debugger), buff), MYDEBUG);
					ok_debugger(&(ts->debugger), DEBUG, "Buffer:", "[" + ok_itostr(ts->sd) + "]" + ok_display_real_buffer(&(ts->debugger), user_ptr->_data), MYDEBUG);
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
					while ((pos = ok_crlf_finder(user_ptr->_data)))
					{
						if (user_ptr->getOverflowFlag())
						{
							user_ptr->_data.erase(user_ptr->_data.begin(), user_ptr->_data.begin() + pos);
							user_ptr->unsetOverflowFlag();
							continue ;
						}
						std::string msg;
						// pos - 2 because CRLF is not needed in the string, it was verified here
						msg.assign(user_ptr->_data.begin(), user_ptr->_data.begin() + pos - 2);
						if (DEEPDEBUG)
						{
							ok_debugger(&(ts->debugger), DEBUG, "Message extracted:", msg, MYDEBUG);
							// std::cout << REDBG_COLOUR "MESSAGE EXTRACTED" NO_COLOUR << std::endl;
							// std::cout << RED_COLOUR << msg << NO_COLOUR << std::endl;
						}
						user_ptr->_data.erase(user_ptr->_data.begin(), user_ptr->_data.begin() + pos);

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
							std::cout << REDBG_COLOUR "MESSAGE" NO_COLOUR << std::endl;
							std::cout << RED_COLOUR << msg << NO_COLOUR << std::endl;
							std::cout << REDBG_COLOUR "NOT VALID, BECAUSE" NO_COLOUR << std::endl;
							std::cout << RED_COLOUR << e.what() << NO_COLOUR << std::endl;
						}
					}


					//if data >BUFFER_SIZE without CRLF
					// clear the data from data
					// set the  data overflow flag
					// if it ends with \r
					//   keep \r in the data

					if (user_ptr->_data.size() > BUFFER_SIZE)
					{
						if (DEEPDEBUG)
							std::cout << RED_COLOUR "DATA IS OVERFLOWING" NO_COLOUR << std::endl;
						user_ptr->setOverflowFlag();
						if (*(user_ptr->_data.rbegin()) == '\r')
						{
							user_ptr->_data.clear();
							user_ptr->_data.push_back('\r');
						}
						else
						{
							user_ptr->_data.clear();
						}
					}
				}
			}
			else if (FD_ISSET(ts->sd , &writefds))
			{
				//send replies
				std::string	outbuff(user_ptr->_data_out.begin(), user_ptr->_data_out.end());
				ok_debugger(&(ts->debugger), DEBUG, "Sending:", "[" + ok_itostr(ts->sd) + "]" + ok_display_send_buffer(ts->debugger.colour, user_ptr->_data_out), MYDEBUG);
				if ((ts->valsent = send(ts->sd , outbuff.c_str(), outbuff.size(), MSG_NOSIGNAL)) <= 0) 
				{ 
					//Somebody disconnected , get his details and print 
					// getpeername(ts->sd, (struct sockaddr*)&ts->address, (socklen_t*)&ts->addrlen); 
					// std::cout << "Host disconnected , ip is : " << inet_ntoa(ts->address.sin_addr)
					// 	<< " , port : " << ntohs(ts->address.sin_port) << std::endl; 
					//Close the socket and mark as 0 in list for reuse
					ok_debugger(&(ts->debugger), DEBUG, "Closing connection on sd: ", ok_itostr(ts->sd), MYDEBUG);
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
					user_ptr->_data_out.erase(user_ptr->_data_out.begin(), user_ptr->_data_out.begin() + ts->valsent);
					//deque would be better for erasing front chunks
					//ok_debugger(&(ts->debugger), DEBUG, "After sending:", "[" + ok_itostr(ts->sd) + "]" + ok_display_real_buffer(&(ts->debugger), user_ptr->_data_out), MYDEBUG);
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
					ok_debugger(&(ts->debugger), DEBUG, "Executing command:", "[" + ok_itostr(msg_ptr->getSD()) + "]" + ok_display_message(&(ts->debugger), msg_ptr->getMessage()), MYDEBUG);
					//maybe run in try and catch block
					ts->commands[msg_ptr->getCommand()](msg_ptr, ts);
				}
				else
				{
					std::cout << RED_COLOUR "Command: " REDBG_COLOUR << msg_ptr->getCommand() << NO_COLOUR RED_COLOUR " not found." NO_COLOUR << std::endl;
					//strike count of invalid messages
				}
				// std::cout << "deleting message " << msg_ptr->getCommand() << std::endl;
				delete msg_ptr;
				temp_it = iter++;
				ts->messages.erase(temp_it);
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
			std::cout << MAGENTA_COLOUR "Removing connection on sd: " << it->first
			<< ". IP: " << it->second->getIP()
			<< ", port: " << it->second->getPort()
			<< NO_COLOUR << std::endl;
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
			std::cout << MAGENTA_COLOUR "Removing message on sd: " << it->first
			<< NO_COLOUR << std::endl;
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
			std::cout << MAGENTA_COLOUR "Removing channel: " << it->first
			<< NO_COLOUR << std::endl;
		}
		delete it->second;
		std::map<std::string, Channel *>::iterator temp = it;
		it++;
		ts->channels.erase(temp);
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
				std::cout << "Invalid DEBUG_LVL: " << value << std::endl;
		}
		else if (key == "DEBUG_FD")
		{
			if (value == "1")
				ts->debugger.fd = 1;
			else if (value == "2")
				ts->debugger.fd = 2;
			else
				std::cout << "Invalid DEBUG_FD: " << value << std::endl;
		}
		else if (key == "DEBUG_DATE")
		{
			if (value == "0")
				ts->debugger.date = false;
			else if (value == "1")
				ts->debugger.date = true;
			else
				std::cout << "Invalid DEBUG_DATE: " << value << std::endl;
		}
		else if (key == "DEBUG_TIME")
		{
			if (value == "0")
				ts->debugger.time = false;
			else if (value == "1")
				ts->debugger.time = true;
			else
				std::cout << "Invalid DEBUG_TIME: " << value << std::endl;
		}
		else if (key == "DEBUG_UTIME")
		{
			if (value == "0")
				ts->debugger.utime = false;
			else if (value == "1")
				ts->debugger.utime = true;
			else
				std::cout << "Invalid DEBUG_UTIME: " << value << std::endl;
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
				std::cout << "Invalid DEBUG_UPRECISION: " << value << std::endl;
		}
		else if (key == "DEBUG_COLOUR")
		{
			if (value == "0")
				ts->debugger.colour = false;
			else if (value == "1")
				ts->debugger.colour = true;
			else
				std::cout << "Invalid DEBUG_COLOUR: " << value << std::endl;
		}
		else if (key == "DEBUG_EXTRA")
		{
			if (value == "0")
				ts->debugger.extra = false;
			else if (value == "1")
				ts->debugger.extra = true;
			else
				std::cout << "Invalid DEBUG_EXTRA: " << value << std::endl;
		}
		else
				std::cout << "Unknown key: " << key << std::endl;
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
