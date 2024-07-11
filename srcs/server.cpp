//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 

#include "server.hpp"
#include "colours.hpp"

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

void	server_loop(t_server ts)
{
	//set of socket descriptors 
	fd_set readfds;
	fd_set writefds;
	//CHECK IF NEEDED TO DO EVERY LOOP BECAUSE OF SELECT
	FD_ZERO(&readfds);
	FD_ZERO(&writefds); 
	FD_SET(ts.master_socket, &readfds);
	while(g_server_alive) 
	{
		if (DEBUG)
			std::cout << WHITEBG_COLOUR "While loop start" NO_COLOUR << MYENDL;
		//clear the socket set 
		FD_ZERO(&readfds);
		FD_ZERO(&writefds); 
		//add master socket to set 
		FD_SET(ts.master_socket, &readfds); 
		ts.max_sd = ts.master_socket; 

		//add connection sockets to set
		for (std::map<int, Connection *>::iterator it = ts.connections.begin(); it != ts.connections.end(); it++)
		{
			//socket descriptor
			ts.sd = it->first;
			if (DEBUG)
				std::cout << BLUE_COLOUR "First for loop sd" << ts.sd << NO_COLOUR << std::endl;
			//if valid socket descriptor then add to read list
			if(ts.sd > 0)
			{
				FD_SET(ts.sd , &readfds);
				std::cout << YELLOW_COLOUR "setting sd: " << ts.sd << NO_COLOUR << MYENDL;
			}
			// IF WE NEED SEND MESSAGE ADD TO WRITE FDS
			//highest file descriptor number, need it for the select function 
			if(ts.sd > ts.max_sd) 
				ts.max_sd = ts.sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is not NULL
		// so it is non blocking
		//timeout set for 15 s 0 ms
		ts.timeout.tv_sec = 15;
		ts.timeout.tv_usec = 0;
		std::cout << "before select" << std::endl;
		//function to find nfds goes here
		// MAX(ts.master_socket, CONNECTIONS-highest key) + 1
		// nfds   This argument should be set to the highest-numbered file 
		// descriptor in any of the three sets, plus 1. The indicated file
		// descriptors in each set are checked, up to this limit (but see
		// BUGS).
		ts.activity = select(ts.max_sd + 1, &readfds , &writefds , NULL , &ts.timeout); 
		std::cout << WHITE_COLOUR "after select "<< ts.activity << NO_COLOUR << MYENDL;
		//check later if allowed
		if (ts.activity < 0) 
		{ 
			std::cerr << ERROR_COLOUR "select error" NO_COLOUR << std::endl;
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
				std::cerr << "accept failed" << std::endl;
				// PROPER_EXIT
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			//Information in constructor
			ts.connections.insert(std::pair<int, Connection*>(ts.new_socket, new Connection(ts.new_socket, ntohs(ts.address.sin_port), inet_ntoa(ts.address.sin_addr))));
			//send new connection greeting message 
			// if(send(ts.new_socket, MESSAGE, std::strlen(MESSAGE), 0) != (ssize_t)std::strlen(MESSAGE) ) 
			// { 
			// 	std::cerr << "send failed" << std::endl;
			// } 
				
			// std::cout << "Welcome message sent successfully" << std::endl;
		} 
			
		//else its some IO operation on some other socket

		//ITERATE OVER CONNECTIONS
		// READING AND SENDING LOOP
		for (std::map<int, Connection *>::iterator it = ts.connections.begin(); it != ts.connections.end(); /*iterating in loop*/)
		{
			//it++ and temp here;
			ts.sd = it->first; 
			if (DEBUG)
				std::cout << BLUE_COLOUR "Second for loop. SD: " << ts.sd << NO_COLOUR << std::endl;
			//READ FROM READING FDS
			if (FD_ISSET(ts.sd , &readfds)) 
			{
				std::cout << YELLOW_COLOUR "sd is set: " << ts.sd << NO_COLOUR << MYENDL;
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((ts.valread = recv(ts.sd , ts.buffer, 512, MSG_NOSIGNAL)) <= 0) 
				{ 
					//Somebody disconnected , get his details and print 
					// getpeername(ts.sd, (struct sockaddr*)&ts.address, (socklen_t*)&ts.addrlen); 
					// std::cout << "Host disconnected , ip is : " << inet_ntoa(ts.address.sin_addr)
					// 	<< " , port : " << ntohs(ts.address.sin_port) << std::endl; 
					//Close the socket and mark as 0 in list for reuse
					if (DEBUG)
						std::cout << "Closing connection on sd: " << ts.sd << std::endl;
					close(ts.sd);
					//REMOVE CONNECTION FROM MAP
					delete it->second;
					std::map<int, Connection *>::iterator temp = it;
					it++;
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
						it->second->_data.push_back(ts.buffer[i]);
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
					while ((pos = ok_crlf_finder(it->second->_data)))
					{
						if (it->second->getOverflowFlag())
						{
							it->second->_data.erase(it->second->_data.begin(), it->second->_data.begin() + pos);
							it->second->unsetOverflowFlag();
							continue ;
						}
						std::string msg;
						// pos - 2 because CRLF is not needed in the string, it was verified here
						msg.assign(it->second->_data.begin(), it->second->_data.begin() + pos - 2);
						if (DEBUG)
						{
							std::cout << REDBG_COLOUR "MESSAGE EXTRACTED" NO_COLOUR << std::endl;
							std::cout << RED_COLOUR << msg << NO_COLOUR << std::endl;
						}
						if (DEBUG > 1)
						{
							for (size_t i = 0; i < msg.length(); i++)
								std::cout << RED_COLOUR << (int)msg[i] << NO_COLOUR << std::endl;
						}
						it->second->_data.erase(it->second->_data.begin(), it->second->_data.begin() + pos);

						// try construct message
						// if success
						//   add to multimap
						// catch exception and throw away
						try
						{
							ts.messages.insert(std::pair<int, Message*>(ts.sd, new Message(ts.sd, msg)));
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

					if (it->second->_data.size() > 512)
					{
						if (DEBUG)
							std::cout << RED_COLOUR "DATA IS OVERFLOWING" NO_COLOUR << std::endl;
						it->second->setOverflowFlag();
						if (*(it->second->_data.rbegin()) == '\r')
						{
							it->second->_data.clear();
							it->second->_data.push_back('\r');
						}
						else
						{
							it->second->_data.clear();
						}
					}
					// if (DEBUG)
					// 	std::cout << "before send " << i << std::endl;
					// //MSG_NOSIGNAL flag added to preent server dying on SIG_PIPE signal from send when socket is closed
					// if (send(ts.sd, ts.buffer, strlen(ts.buffer), MSG_NOSIGNAL) == -1)
					// {
					// 	if (DEBUG)
					// 		std::cerr << "send failed" << i << std::endl;
					// }
					it++;
				}
			}
			else if (FD_ISSET(ts.sd , &writefds))
			{
				// SEND TO WRITING FDS
				// SEND WITH YELLOW COLOUR
				it++;
			}
			else
			{
				// READ FROM BUFFER STRING IF IT EXISTS
				it++;
			}
		} 
	}
	if (DEBUG)
		std::cout << "Main while loop ended..." << std::endl;
	// ITERATE OVER MAP AND DELETE EVERYTHING
	for (std::map<int, Connection *>::iterator it = ts.connections.begin(); it != ts.connections.end(); /*iterating in loop*/)
	{
		if (DEBUG)
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
}

// create map of commands and pointers to functions to process them
void	init_server(t_server ts)
{
	ts.opt = TRUE;
	//create a master socket 
	if( (ts.master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(ts.master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&ts.opt, 
		sizeof(ts.opt)) < 0 ) 
	{ 
		std::cerr << "setsockopt" << std::endl;
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	ts.address.sin_family = AF_INET; 
	ts.address.sin_addr.s_addr = INADDR_ANY; 
	ts.address.sin_port = htons(ts.port); 
		
	//bind the socket to localhost port 8888 
	if (bind(ts.master_socket, (struct sockaddr *)&ts.address, sizeof(ts.address))<0) 
	{ 
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE); 
	} 
	std::cout << "Listener on port " << ts.port << std::endl; 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(ts.master_socket, 3) < 0) 
	{ 
		std::cerr << "listen" << std::endl;
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	ts.addrlen = sizeof(ts.address); 
	std::cout << "Waiting for connections ..." << std::endl; 

	//main server loop
	server_loop(ts);
}

int	ft_usage(void)
{
	std::cerr << ERROR_COLOUR "Usage:" NO_COLOUR << std::endl
		<< REDBG_COLOUR "./ircserv <port> <password>" NO_COLOUR << std::endl;
	return (1);
}

int	ft_usage_port(void)
{
	std::cerr << ERROR_COLOUR "Invalid port:" NO_COLOUR << std::endl
		<< REDBG_COLOUR "port has to be in the range of 1-65535" NO_COLOUR << std::endl;
	return (2);
}

static int	ok_strtoi(std::string str)
{
	std::stringstream	temp;
	int					num;

	temp << str;
	temp >> num;
	return (num);
}

int	main(int argc , char *argv[]) 
{
	if (argc != 3)
		return (ft_usage());
	//check argv1
	//check argv2?
	signal(SIGINT, signal_handler); 
	t_server	ts;
	ts.port = ok_strtoi(argv[1]);
	if (ts.port <=0 || ts.port > 65535)
		return (ft_usage_port());
	ts.password = argv[2];
	init_server(ts);
	return 0; 
} 
