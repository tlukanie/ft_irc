/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:16:33 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 16:16:34 by tlukanie         ###   ########.fr       */
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
