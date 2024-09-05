//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 

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
		// ERR_NEEDMOREPARAMS (461)
		// incorrect message count
		std::cerr << RED_COLOUR "no parameter" NO_COLOUR << std::endl;
		return ;
	}
	// ERR_ALREADYREGISTERED (462)
	if (msg->getParams()[0] == ts->password)
	{
		std::cout << GREEN_COLOUR "correct password" NO_COLOUR << std::endl; 
	}
	else
	{
		// ERR_PASSWDMISMATCH (464)
		std::cerr << RED_COLOUR "wrong password" NO_COLOUR << std::endl;
		reply = ":IRC 464  :Wrong password try again" CRLF;
		std::cout << YELLOWBG_COLOUR << reply << NO_COLOUR << std::endl;
		
		if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
		{ 
			std::cerr << "send failed" << std::endl;
		} 
		else
		{
			std::cout << "Reply message sent successfully" << std::endl;
		}
		close(msg->getSD());
		ts->connections.erase(ts->connections.find(msg->getSD()));
	}
	
}

// NICK net
// https://modern.ircdocs.horse/#nick-message
// handle existing nick later
// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

// Numeric Replies:

// ERR_NONICKNAMEGIVEN (431)
// ERR_ERRONEUSNICKNAME (432)
// ERR_NICKNAMEINUSE (433)
// ERR_NICKCOLLISION (436)
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

	std::cout << MAGENTA_COLOUR "NICK COMMAND is not fully supported" NO_COLOUR << std::endl;
	// later check if nick is in use and valid string

	//setting the first (and only?) parameter as a nick in the map of connections
	//accessed by the socket descriptor gained from the message class
	std::cout << MAGENTA_COLOUR "Connection " << msg->getSD()
	<< " setting nick " << msg->getParams()[0] << NO_COLOUR << std::endl;

	ts->connections[msg->getSD()]->setNick(msg->getParams()[0]);

	if (ts->connections[msg->getSD()]->getNick().size())
	{
		//send to all?
		//:net!net@127.0.0.1 << need to be not hardcoded
		reply = ":net!net@127.0.0.1 NICK :" +  ts->connections[msg->getSD()]->getNick() + CRLF;
		std::cout << YELLOWBG_COLOUR << reply << NO_COLOUR << std::endl;
		if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
		{ 
			std::cerr << "send failed" << std::endl;
		} 
		else
		{
			std::cout << "Reply message sent successfully" << std::endl;
		}
	}

	std::cout << MAGENTA_COLOUR "Connection " << msg->getSD()
	<< " got nick " << ts->connections[msg->getSD()]->getNick() << NO_COLOUR << std::endl;
}

// USER net net localhost :net
// https://modern.ircdocs.horse/#user-message
// reply :IRCQ+ 001 net :Welcome to IRCQ+ net!net@127.0.0.1
//net!net@127.0.0.1 <<<< save this string somehow? Create function to make it?
//Numeric Replies:

// ERR_NEEDMOREPARAMS (461)
// ERR_ALREADYREGISTERED (462)
void irc_user(Message* msg, struct s_server *ts)
{
	std::string	reply;
	std::cout << MAGENTA_COLOUR "USER COMMAND is not fully supported" NO_COLOUR << std::endl;
	reply = "001 " +  ts->connections[msg->getSD()]->getNick() + " :Hello there" + CRLF;
	std::cout << YELLOWBG_COLOUR << reply << NO_COLOUR << std::endl;
	if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
	{ 
		std::cerr << "send failed" << std::endl;
	} 
	else
	{
		std::cout << "Reply message sent successfully" << std::endl;
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

	std::cout << MAGENTA_COLOUR "PING COMMAND is almost supported" NO_COLOUR << std::endl; 
	
	reply = "PONG " +  ts->connections[msg->getSD()]->getNick() + " :" + msg->getParams()[0] + CRLF;
	std::cout << YELLOWBG_COLOUR << reply << NO_COLOUR << std::endl;

	if(send(msg->getSD(), reply.c_str(), reply.length(), 0) != (ssize_t)reply.length())
	{ 
		std::cerr << "send failed" << std::endl;
	} 
	else
	{
		std::cout << "Reply message sent successfully" << std::endl;
	}
}

// PONG
// https://modern.ircdocs.horse/#pong-message
void irc_pong(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "PONG COMMAND not supported" NO_COLOUR << std::endl; 
	(void)msg;
	(void)ts;
}

// QUIT
// https://modern.ircdocs.horse/#quit-message
void irc_quit(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "QUIT COMMAND not supported" NO_COLOUR << std::endl; 
	(void)msg;
	(void)ts;
}

// Error
// https://modern.ircdocs.horse/#error-message

/* CHANNEL OPERATIONS */

// JOIN
// https://modern.ircdocs.horse/#join-message

// PART
//https://modern.ircdocs.horse/#part-message

//TOPIC
// https://modern.ircdocs.horse/#topic-message

//INVITE
// https://modern.ircdocs.horse/#invite-message

//KICK
//https://modern.ircdocs.horse/#kick-message


//AWAY
//https://modern.ircdocs.horse/#away-message



// MODE net +i
//https://modern.ircdocs.horse/#mode-message
//https://modern.ircdocs.horse/#channel-modes


//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message

//NOTICE
//https://modern.ircdocs.horse/#notice-message


void irc_mode(Message* msg, struct s_server *ts)
{
	std::cout << MAGENTA_COLOUR "MODE COMMAND not supported" NO_COLOUR << std::endl; 
	(void)msg;
	(void)ts;
}


void	server_loop(t_server ts)
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
			ok_debugger(&(ts.debugger), DEBUG, "While loop started...", "", MYDEBUG);
		//clear the socket set 
		FD_ZERO(&readfds);
		FD_ZERO(&writefds); 
		//add master socket to set 
		FD_SET(ts.master_socket, &readfds); 
		ts.max_sd = ts.master_socket; 

		//add connection sockets to set
		//in the reading loop
		// if (ts.state & READING_LOOP)
		// {
		Connection * connection_ptr;
		for (std::map<int, Connection *>::iterator it = ts.connections.begin(); it != ts.connections.end(); it++)
		{
			//socket descriptor
			ts.sd = it->first;
			connection_ptr = it->second;
			if (DEEPDEBUG)
				ok_debugger(&(ts.debugger), DEBUG, "First for loop reading sd ", ok_itostr(ts.sd), MYDEBUG);
			if (connection_ptr->getReadingFlag())
			{
				FD_SET(ts.sd , &readfds);
				if (DEEPDEBUG)
					ok_debugger(&(ts.debugger), DEBUG, "Setting reading sd: ", ok_itostr(ts.sd), MYDEBUG);
			}
			else
			{
				FD_SET(ts.sd , &writefds);
				if (DEEPDEBUG)
					ok_debugger(&(ts.debugger), DEBUG, "Setting writing sd: ", ok_itostr(ts.sd), MYDEBUG);
			}
			if(ts.sd > ts.max_sd) 
				ts.max_sd = ts.sd; 
		}
		ok_debugger(&(ts.debugger), EXTRADEBUG, "Before select", "", MYDEBUG);
		//function to find nfds goes here
		// MAX(ts.master_socket, CONNECTIONS-highest key) + 1
		// nfds   This argument should be set to the highest-numbered file 
		// descriptor in any of the three sets, plus 1. The indicated file
		// descriptors in each set are checked, up to this limit (but see
		// BUGS).
		ts.timeout.tv_sec = 15;
		ts.timeout.tv_usec = 0;
		ts.activity = select(ts.max_sd + 1, &readfds , &writefds , NULL , &ts.timeout);
		ok_debugger(&(ts.debugger), EXTRADEBUG, "After select", "", MYDEBUG);
		//check later if allowed
		if (ts.activity < 0) 
		{ 
			ok_debugger(&(ts.debugger), ERROR, "Select error", ok_itostr(errno), MYDEBUG);
			continue ;
		}
		// IF SELECT RETURNS 0  MAYBE CONTINUE ???
		// NEED TO CHECK THAT NO UNPROCESSED MESSAGES IN BUFFER
		else if (!ts.activity)
			continue ;
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(ts.master_socket, &readfds)) 
		{ 
			if ((ts.new_socket = accept(ts.master_socket, 
					(struct sockaddr *)&ts.address, (socklen_t*)&ts.addrlen))<0) 
			{ 
				ok_debugger(&(ts.debugger), ERROR, "Accept failed", ok_itostr(errno), MYDEBUG);
				// PROPER_EXIT
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			//Information in constructor
			ts.connections.insert(std::pair<int, Connection*>(ts.new_socket, new Connection(ts.new_socket, ntohs(ts.address.sin_port), inet_ntoa(ts.address.sin_addr))));
		} 
			
		//else its some IO operation on some other socket

		//ITERATE OVER CONNECTIONS
		// READING AND SENDING LOOP
		// if (ts.state & READING_LOOP)
		// {
		for (std::map<int, Connection *>::iterator it = ts.connections.begin(); it != ts.connections.end(); /*iterating in loop*/)
		{
			std::map<int, Connection *>::iterator temp = it;
			ts.sd = it->first;
			Connection * user_ptr = it->second;
			it++;
			if (DEEPDEBUG)
				ok_debugger(&(ts.debugger), DEBUG, "Reading for loop ", ok_itostr(ts.sd), MYDEBUG);
			//READ FROM READING FDS
			if (FD_ISSET(ts.sd , &readfds)) 
			{
				ok_debugger(&(ts.debugger), DEBUG, "SD is set: ", ok_itostr(ts.sd), MYDEBUG);
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((ts.valread = recv(ts.sd , ts.buffer, 512, MSG_NOSIGNAL)) <= 0) 
				{ 
					//Somebody disconnected , get his details and print 
					// getpeername(ts.sd, (struct sockaddr*)&ts.address, (socklen_t*)&ts.addrlen); 
					// std::cout << "Host disconnected , ip is : " << inet_ntoa(ts.address.sin_addr)
					// 	<< " , port : " << ntohs(ts.address.sin_port) << std::endl; 
					//Close the socket and mark as 0 in list for reuse
					if (DEEPDEBUG)
						ok_debugger(&(ts.debugger), DEBUG, "Closing connection on sd: ", ok_itostr(ts.sd), MYDEBUG);
					close(ts.sd);
					//REMOVE CONNECTION FROM MAP
					delete user_ptr;
					ts.connections.erase(temp);
				} 
				//Print the message that came in 
				else
				{
					std::cout << "Received data" << std::endl;
					std::cout << "Buffer on sd " << ts.sd << " [" CYAN_COLOUR;
					for (int i = 0; i < ts.valread; i++)
					{
						std::cout << ts.buffer[i];
						user_ptr->_data.push_back(ts.buffer[i]);
					}
					std::cout << NO_COLOUR "]" << std::endl;


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
							std::cout << REDBG_COLOUR "MESSAGE EXTRACTED" NO_COLOUR << std::endl;
							std::cout << RED_COLOUR << msg << NO_COLOUR << std::endl;
						}
						user_ptr->_data.erase(user_ptr->_data.begin(), user_ptr->_data.begin() + pos);

						// try construct message
						// if success
						//   add to multimap
						// catch exception and throw away
						try
						{
							ts.messages.insert(std::pair<int, Message*>(ts.sd, new Message(ts.sd, msg)));
							// ts.state = SENDING_LOOP;
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


					//if data >512 without CRLF
					// clear the data from data
					// set the  data overflow flag
					// if it ends with \r
					//   keep \r in the data

					if (user_ptr->_data.size() > 512)
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
			else if (FD_ISSET(ts.sd , &writefds))
			{
				// SEND TO WRITING FDS
				// SEND WITH YELLOW COLOUR
					// for (std::multimap<int, Message*>::iterator it = ts.messages.begin(); it != ts.messages.end(); /*iterating in the loop */)
					// {
				std::multimap<int, Message*>::iterator iter;
				iter = ts.messages.find(ts.sd);
				//if we processed all messages
				if (iter == ts.messages.end())
				{
					user_ptr->setReadingFlag();
					continue ;
				}
				Message * msg_ptr = iter->second;

				// find if the command is in commands
				//	execute
				if (ts.commands.find(msg_ptr->getCommand()) != ts.commands.end())
				{
					std::cout << "Executing message " << msg_ptr->getCommand() << std::endl;
					//maybe run in try and catch block
					ts.commands[msg_ptr->getCommand()](msg_ptr, &ts);
				}
				else
				{
					std::cout << RED_COLOUR "Command: " REDBG_COLOUR << msg_ptr->getCommand() << NO_COLOUR RED_COLOUR " not found." NO_COLOUR << std::endl;
					//strike count of invalid messages
				}
				std::cout << "deleting message " << msg_ptr->getCommand() << std::endl;
				delete msg_ptr;
				ts.messages.erase(iter);
			}
			else
			{
				//fd is not ready
				//check for pending messages and timeout???
			}
		}
	}
	if (DEEPDEBUG)
		ok_debugger(&(ts.debugger), WARNING, "Main loop terminating...", "", MYDEBUG);
	// ITERATE OVER MAP AND DELETE EVERYTHING
	for (std::map<int, Connection *>::iterator it = ts.connections.begin(); it != ts.connections.end(); /*iterating in the loop*/)
	{
		if (DEEPDEBUG)
		{
			std::cout << MAGENTA_COLOUR "Removing connection on sd: " << it->first
			<< ". IP: " << it->second->getIP()
			<< ", port: " << it->second->getPort()
			<< NO_COLOUR << std::endl;
		}
		delete it->second;
		std::map<int, Connection *>::iterator temp = it;
		it++;
		ts.connections.erase(temp);
	}
	for (std::multimap<int, Message*>::iterator it = ts.messages.begin(); it != ts.messages.end(); /*iterating in the loop*/)
	{
		if (DEEPDEBUG)
		{
			std::cout << MAGENTA_COLOUR "Removing message on sd: " << it->first
			<< NO_COLOUR << std::endl;
		}
		delete it->second;
		std::multimap<int, Message*>::iterator temp = it;
		it++;
		ts.messages.erase(temp);
	}
}

// create map of commands and pointers to functions to process them
void	init_server(t_server ts)
{
	ts.opt = TRUE;
	//create a master socket 
	if( (ts.master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{
		ok_debugger(&(ts.debugger), ERROR, "Socket failed", ok_itostr(errno), MYDEBUG);
		exit(EXIT_FAILURE); 
	} 
	
	//server starts by listening for new connections
	ts.state = READING_LOOP;

	//wait for an activity on one of the sockets , timeout is not NULL
	// so it is non blocking
	//timeout set for 15 s 0 ms
	ts.timeout.tv_sec = 15;
	ts.timeout.tv_usec = 0;

	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(ts.master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&ts.opt, 
		sizeof(ts.opt)) < 0 ) 
	{ 
		ok_debugger(&(ts.debugger), ERROR, "Setsockopt failed", ok_itostr(errno), MYDEBUG);
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	ts.address.sin_family = AF_INET; 
	ts.address.sin_addr.s_addr = INADDR_ANY; 
	ts.address.sin_port = htons(ts.port); 
		
	//bind the socket to localhost port 8888 
	if (bind(ts.master_socket, (struct sockaddr *)&ts.address, sizeof(ts.address))<0) 
	{ 
		ok_debugger(&(ts.debugger), ERROR, "Bind failed", ok_itostr(errno), MYDEBUG);
		exit(EXIT_FAILURE); 
	}
	ok_debugger(&(ts.debugger), INFO, std::string("Listening on port: ") + ok_itostr(ts.port), "", MYDEBUG);

	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(ts.master_socket, 3) < 0) 
	{ 
		ok_debugger(&(ts.debugger), ERROR, "Listen failed", ok_itostr(errno), MYDEBUG);
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	ts.addrlen = sizeof(ts.address); 
	ok_debugger(&(ts.debugger), INFO, "Waiting for connections ...", "", MYDEBUG);

	//add commands
	ts.commands["CAP"] = irc_cap;
	ts.commands["PASS"] = irc_pass;
	ts.commands["NICK"] = irc_nick;
	ts.commands["USER"] = irc_user;
	ts.commands["PING"] = irc_ping;
	ts.commands["PONG"] = irc_pong;
	ts.commands["MODE"] = irc_mode;
	
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;
	// ts.commands[""] = irc_;

	// ok_debugger(&(ts.debugger), DEBUG, "This is a debug message", "", MYDEBUG);
	// ok_debugger(&(ts.debugger), INFO, "This is an info message", "", MYDEBUG);
	// ok_debugger(&(ts.debugger), NOTICE, "This is a notice message", "", MYDEBUG);
	// ok_debugger(&(ts.debugger), WARNING, "This is a warning message", "", MYDEBUG);
	// ok_debugger(&(ts.debugger), ERROR, "This is an error message", "", MYDEBUG);
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

int	main(int argc , char *argv[]) 
{
	if (argc < 3 || argc > 4)
		return (ft_usage());
	//check argv1
	//check argv2?
	signal(SIGINT, signal_handler); 
	t_server	ts;
	ts.port = ok_strtoi<int>(argv[1]);
	if (ts.port <=0 || ts.port > 65535)
		return (ft_usage_port());
	if (!ok_argvcheck(argv[3], &ts))
		return (ft_usage_debug());
	ft_init_debugger(&(ts.debugger));
	ok_debugger(&(ts.debugger), NOTICE, std::string("DEBUG LEVEL: ") + ok_itostr(ts.debugger.debuglvl), "", MYDEBUG);
	ts.password = argv[2];
	init_server(ts);
	//try init server
	// catch
	//	return 1;
	return 0; 
} 
