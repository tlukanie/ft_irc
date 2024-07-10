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
			ts.sd = it->first; 
			if (DEBUG)
				std::cout << BLUE_COLOUR "Second for loop. SD: " << ts.sd << NO_COLOUR << std::endl;
			//READ FROM READING FDS
			if (FD_ISSET(ts.sd , &readfds)) 
			{
				std::cout << YELLOW_COLOUR "sd is set: " << ts.sd << NO_COLOUR << MYENDL;
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((ts.valread = recv( ts.sd , ts.buffer, 1024, MSG_NOSIGNAL)) <= 0) 
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
					//set the string terminating NULL byte on the end 
					//of the data read
					ts.buffer[ts.valread] = '\0';
					std::cout << "Received data" << std::endl;
					std::cout << "Buffer on sd " << ts.sd << " [" CYAN_COLOUR << ts.buffer << NO_COLOUR "]" << std::endl;
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
