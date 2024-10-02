/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic8bot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 13:35:07 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 11:24:44 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"


static bool		g_client_alive = true;

void signal_handler(int signal_num) 
{
	if (signal_num == SIGINT)
		g_client_alive = false;
}



void	client_read(t_client *tc)
{
	char	buff[512];
	int		r;
	r = read(STDIN_FILENO, buff, 511);
	for (int i = 0; i < r; i++)
		tc->messageOut += buff[i];
	if (tc->messageOut.size() && (*(tc->messageOut.rbegin()) == '\n' || *(tc->messageOut.rbegin()) == '\0'))
	{
		tc->messageOut.erase(tc->messageOut.size() - 1);
		tc->messageOut += CRLF;
	}
	if (r > 0)
	{
		buff[r] = 0;
		tc->ready = true;
		ok_debugger(&(tc->debugger), DEBUG, "Buffer read:",  ok_display_buffer(&(tc->debugger), buff), MYDEBUG);
	}
}

void	client_write(t_client *tc)
{
	int		w;

	w = write(STDOUT_FILENO, tc->messageIn.c_str(), tc->messageIn.size());
	(void)w;
	tc->messageIn.clear();
}

void	client_recv(t_client *tc)
{
	int		rc;

	if ((rc = recv(tc->clientSocket, tc->buffer, 512, MSG_NOSIGNAL)) <= 0) 
	{ 
		//end bot?
	} 
	//Print the message that came in 
	else
	{
		std::string	buff;
		for (int i = 0; i < rc; i++)
		{
			tc->dataIn.push_back(tc->buffer[i]);
			buff.push_back(tc->buffer[i]);
		}
		// ok_debugger(&(ts->debugger), DEBUG, "Buffer:", ok_display_buffer(&(tc->debugger), buff), MYDEBUG);
		ok_debugger(&(tc->debugger), DEBUG, "Buffer:", ok_display_real_buffer(&(tc->debugger), tc->dataIn), MYDEBUG);
		size_t	pos;
		while ((pos = ok_crlf_finder(tc->dataIn)))
		{
			if (tc->dataInOverflow)
			{
				tc->dataIn.erase(tc->dataIn.begin(), tc->dataIn.begin() + pos);
				tc->dataInOverflow = false;
				continue ;
			}
			std::string msg;
			// pos - 2 because CRLF is not needed in the string, it was verified here
			msg.assign(tc->dataIn.begin(), tc->dataIn.begin() + pos - 2);
			if (DEEPDEBUG)
			{
				ok_debugger(&(tc->debugger), DEBUG, "Message extracted:", msg, MYDEBUG);
			}
			tc->dataIn.erase(tc->dataIn.begin(), tc->dataIn.begin() + pos);
			try
			{
				tc->dataInOverflow = false;
				Message *msg_ptr = new Message(msg);
				tc->messages.push_back(msg_ptr);
			}
			catch(const std::exception& e)
			{
				ok_debugger(&(tc->debugger), ERROR, "Message: " + msg + " is not valid, because: ", e.what(), MYDEBUG);
			}
		}
		if (tc->dataIn.size() > BUFFER_SIZE)
		{
			if (DEEPDEBUG)
				ok_debugger(&(tc->debugger), ERROR, "Data is overflowing", "", MYDEBUG);
			tc->dataInOverflow = true;
			if (*(tc->dataIn.rbegin()) == '\r')
			{
				tc->dataIn.clear();
				tc->dataIn.push_back('\r');
			}
			else
			{
				tc->dataIn.clear();
			}
		}
	}

}

void	client_send(t_client *tc)
{
	// std::string	message = "PASS " + tc->password + CRLF "NICK " + tc->botname + CRLF "USER a b c d" CRLF;
	send(tc->clientSocket, tc->messageOut.c_str(), tc->messageOut.size(), MSG_NOSIGNAL);
	ok_debugger(&(tc->debugger), INFO, "Message sent:",  ok_display_buffer(&(tc->debugger), tc->messageOut), MYDEBUG);
	tc->messageOut.clear();
	tc->ready = false;
}

void	client_loop(t_client *tc)
{
	//set of socket descriptors 
	fd_set readfds;
	fd_set writefds;
	// //NEEDED TO DO EVERY LOOP BECAUSE OF SELECT
	// FD_ZERO(&readfds);
	// FD_ZERO(&writefds); 
	// FD_SET(tc.master_socket, &readfds);
	while(g_client_alive) 
	{
		// if (DEEPDEBUG)
		// 	ok_debugger(&(tc->debugger), DEBUG, "While loop started...", "", MYDEBUG);
		//clear the socket set 
		FD_ZERO(&readfds);
		FD_ZERO(&writefds); 
		//add master socket to set 
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(STDOUT_FILENO, &writefds);
		if (!tc->ready)
		{
			FD_SET(tc->clientSocket, &readfds);
		}
		else
		{
			FD_SET(tc->clientSocket, &writefds);
		}
		tc->max_sd = tc->clientSocket; 
		if (tc->debugger.log.size())
		{
			FD_SET(tc->debugger.fd, &writefds);
			if(tc->debugger.fd > tc->max_sd)
				tc->max_sd = tc->debugger.fd;
		}
		
		tc->timeout.tv_sec = 15;
		tc->timeout.tv_usec = 0;
		tc->activity = select(tc->max_sd + 1, &readfds , &writefds , NULL , &tc->timeout);
		// ok_debugger(&(tc->debugger), DEBUG, "After select", "", MYDEBUG);
		//check later if allowed
		if (tc->activity < 0) 
		{
			ok_debugger(&(tc->debugger), ERROR, "Select error", ok_itostr(errno), MYDEBUG);
			continue ;
		}
		else if (!tc->activity)
			continue ;
		//If something happened on the master socket , 
		//then itc an incoming connection
		if (FD_ISSET(STDIN_FILENO, &readfds)) 
			client_read(tc);
		if (FD_ISSET(STDOUT_FILENO, &writefds)) 
			client_write(tc);
		if (FD_ISSET(tc->clientSocket, &readfds)) 
			client_recv(tc);
		if (FD_ISSET(tc->clientSocket, &writefds)) 
			client_send(tc);

		//iterate over messages
		{
			// SEND TO WRITING FDS
			// SEND WITH YELLOW COLOUR
				// for (std::multimap<int, Message*>::iterator it = ts->messages.begin(); it != ts->messages.end(); /*iterating in the loop */)
				// {
			std::vector<Message*>::iterator iter;
			std::vector<Message*>::iterator temp_it;
			iter = tc->messages.begin();
			while (iter != tc->messages.end())
			{
				iter++;
			}
			iter = tc->messages.begin();
			while (iter != tc->messages.end())
			{
				Message * msg_ptr = *iter;

				// find if the command is in commands
				//	execute
				if (tc->commands.find(msg_ptr->getCommand()) != tc->commands.end())
				{
					ok_debugger(&(tc->debugger), DEBUG, "Executing command:", ok_display_message(&(tc->debugger), msg_ptr->getCommand()), MYDEBUG);
					//maybe run in try and catch block
					tc->commands[msg_ptr->getCommand()](msg_ptr, tc);
				}
				else
				{
					ok_debugger(&(tc->debugger), WARNING, "Unknown command:", ok_display_message(&(tc->debugger), msg_ptr->getCommand()), MYDEBUG);
					//err_unknowncommand_421(tc, msg_ptr->getSD(), msg_ptr->getCommand());
					//strike count of invalid messages
				}
				delete msg_ptr;
				iter++;
			}
			tc->messages.clear();
		}
		//print debugs
		if (FD_ISSET(tc->debugger.fd , &writefds))
		{
			ssize_t wb;//number of written bytes
			wb = write(tc->debugger.fd, tc->debugger.log.c_str(), tc->debugger.log.size());
			if (wb < 0)
			{
				//error ignored for now
			}
			else if ((size_t)wb <= tc->debugger.log.size())
			{
				tc->debugger.log.erase(0,wb);
			}
			else
			{
				//?????
			}
		}
	}
}

void	init_client(t_client *tc, std::string const &arg, std::string const &arg2)
{
	tc->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	// std::cout << "SOCKET: " << tc->clientSocket << std::endl;
	tc->ready = false;
	tc->dataInOverflow = false;
	//Defining Server Address
	tc->mode = MODE_INTERACTIVE;
	srand(time(0));
	if (arg == "AUTO")
	{
		tc->mode = MODE_AUTOMATED;
		if (arg2.size())
			tc->channel = arg2;
	}
	else if (arg == "TEST")
		tc->mode = MODE_TESTING;
	if (tc->mode == MODE_TESTING)
		tc->test = arg2.size(); //rewrite
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(tc->port);
	// std::cout << "PORT: " << tc->port << std::endl;
	serverAddress.sin_addr.s_addr = inet_addr(tc->serverIP.c_str()); //inet_pton for IPv6
	// std::cout << "IP: " << tc->serverIP << std::endl;
	//Connecting to the Server // check for error
	connect(tc->clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	//Sending Data to the Server
	if (tc->mode)
	{
		tc->messageOut = "PASS " + tc->password + CRLF "NICK " + tc->botname + CRLF "USER a b c d" CRLF;
		std::cout << "HELLLLLOOOO" << tc->mode << arg << arg2 << std::endl;
		if (tc->channel.size())
			tc->messageOut +=  "JOIN " + tc->channel + CRLF;
		tc->ready = true;
	}

	std::string	deck[52] = {"🂡", "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮",
							"🂱", "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾",
							"🃁", "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎",
							"🃑", "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞"};
	for (int i = 0; i < 52; i++)
	{
		tc->cards.push_back(deck[i]);
	}
	//add actions
	tc->actions["!card"] = bot_card;
	tc->actions["!flip"] = bot_flip;
	tc->actions["!rps"] = bot_rps;
	tc->actions["!roll"] = bot_roll;

	//add commands
	// tc->commands["CAP"] = irc_cap;
	// tc->commands["PASS"] = irc_pass;
	// tc->commands["NICK"] = irc_nick;
	// tc->commands["USER"] = irc_user;
	// tc->commands["PING"] = irc_ping;
	// tc->commands["PONG"] = irc_pong;
	// tc->commands["MODE"] = irc_mode;
	// tc->commands["JOIN"] = irc_join;
	// tc->commands["PART"] = irc_part;
	tc->commands["PRIVMSG"] = irc_privmsg;
	// tc->commands["NOTICE"] = irc_notice;
	// tc->commands["TOPIC"] = irc_topic;
	// tc->commands["INVITE"] = irc_invite;
	// tc->commands["KICK"] = irc_kick;
	// tc->commands["AWAY"] = irc_away;
	// tc->commands["WHO"] = irc_who;
	// tc->commands["WHOIS"] = irc_whois; //debug
	// tc->commands["QUIT"] = irc_quit;

	//main client loop
	client_loop(tc);

}

//clean client
void	clean_client(t_client *tc)
{
	std::cout << "helllooooo????" << std::endl;
	if (DEEPDEBUG)
		ok_debugger(&(tc->debugger), WARNING, "Client loop terminating...", "", MYDEBUG);
	if (tc->clientSocket > 0)
		close(tc->clientSocket);
	for (std::vector<Message*>::iterator it = tc->messages.begin(); it != tc->messages.end(); it++)
	{
		if (DEEPDEBUG)
		{
			ok_debugger(&(tc->debugger), INFO, "Removing message: " + (*it)->getMessage(), "", MYDEBUG);
		}
		delete *it;
	}
	ssize_t	wb;
	wb = write(tc->debugger.fd, tc->debugger.log.c_str(), tc->debugger.log.size());
	if (wb < 0)
	{
		std::cerr << "failed write" << std::endl;
	}
	else if ((size_t)wb <= tc->debugger.log.size())
	{
		tc->debugger.log.erase(0,wb);
	}

}

int main(int argc, char *argv[])
{
	t_client	tc;

	signal(SIGINT, signal_handler); 
	irc_init_debugger(&(tc.debugger));
	if (irc_read_client_config(&tc))
		return (1);
	std::string	arg;
	std::string	arg2;
	(void)argc;
	if (argv[1])
	{
		arg = argv[1];
		if (argv[2])
			arg2 = argv[2];
	}
	try
	{
		init_client(&tc, arg, arg2);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		clean_client(&tc);
		return (1);
	}
	std::cerr << "\b\b";
	clean_client(&tc);
	// Creating the Client Socket
	
	
}