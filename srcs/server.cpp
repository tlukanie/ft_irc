//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
	
#define TRUE 1 
#define FALSE 0

#ifndef DEBUG
# define DEBUG 1
#endif

#define MESSAGE "ECHO Daemon v1.0 \r\n"

typedef struct s_server {
	std::string password;
	int	port;
	int	opt;
	int	addrlen;
	int	master_socket;
	int	new_socket;
	int	client_socket[30];
	int	max_clients;
	int	activity;
	int	valread;
	int	sd; 
	int	max_sd; 
	struct sockaddr_in address; 
	char buffer[1025]; //data buffer of 1K 
}	t_server;

void	server_loop(t_server ts)
{
	//set of socket descriptors 
	fd_set readfds;
	while(TRUE) 
	{
		if (DEBUG)
			std::cout << "While loop start" << std::endl;
		//clear the socket set 
		FD_ZERO(&readfds); 
		//add master socket to set 
		FD_SET(ts.master_socket, &readfds); 
		ts.max_sd = ts.master_socket; 
			
		//add child sockets to set 
		for (int i = 0 ; i < ts.max_clients ; i++) 
		{ 
			if (DEBUG)
				std::cout << "First for loop " << i << std::endl;
			//socket descriptor 
			ts.sd = ts.client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(ts.sd > 0) 
				FD_SET(ts.sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(ts.sd > ts.max_sd) 
				ts.max_sd = ts.sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		ts.activity = select(ts.max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		//check later if allowed
		if ((ts.activity < 0) && (errno!=EINTR)) 
		{ 
			std::cerr << "select error" << std::endl; 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(ts.master_socket, &readfds)) 
		{ 
			if ((ts.new_socket = accept(ts.master_socket, 
					(struct sockaddr *)&ts.address, (socklen_t*)&ts.addrlen))<0) 
			{ 
				std::cerr << "accept" << std::endl;
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			std::cout << "New connection , socket fd is " << ts.new_socket
				<< ", ip is : " << inet_ntoa(ts.address.sin_addr)
				<< " , port : " << ntohs(ts.address.sin_port) << std::endl; 
		
			//send new connection greeting message 
			if(send(ts.new_socket, MESSAGE, strlen(MESSAGE), 0) != (ssize_t)strlen(MESSAGE) ) 
			{ 
				std::cerr << "send" << std::endl;
			} 
				
			std::cout << "Welcome message sent successfully" << std::endl;
				
			//add new socket to array of sockets 
			for (int i = 0; i < ts.max_clients; i++) 
			{ 
				if (DEBUG)
					std::cout << "Second for loop " << i << std::endl;
				//if position is empty 
				if( ts.client_socket[i] == 0 ) 
				{ 
					ts.client_socket[i] = ts.new_socket; 
					std::cout << "Adding to list of sockets as " << i << std::endl; 
						
					break; 
				} 
			} 
		} 
			
		//else its some IO operation on some other socket 
		for (int i = 0; i < ts.max_clients; i++) 
		{ 

			if (DEBUG)
				std::cout << "Third for loop " << i << std::endl;
			ts.sd = ts.client_socket[i]; 	
			if (FD_ISSET(ts.sd , &readfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((ts.valread = recv( ts.sd , ts.buffer, 1024, MSG_NOSIGNAL)) <= 0) 
				{ 
					//Somebody disconnected , get his details and print 
					getpeername(ts.sd, (struct sockaddr*)&ts.address, (socklen_t*)&ts.addrlen); 
					std::cout << "Host disconnected , ip is : " << inet_ntoa(ts.address.sin_addr)
						<< " , port : " << ntohs(ts.address.sin_port) << std::endl; 
					//Close the socket and mark as 0 in list for reuse 
					close(ts.sd); 
					ts.client_socket[i] = 0; 
				} 
					
				//Echo back the message that came in 
				else
				{
					//set the string terminating NULL byte on the end 
					//of the data read
					ts.buffer[ts.valread] = '\0';
					std::cout << "Received data" << std::endl;
					std::cout << "Buffer " << i << " [" << ts.buffer << "]" << std::endl;
					if (DEBUG)
						std::cout << "before send " << i << std::endl;
					//MSG_NOSIGNAL flag added to preent server dying on SIG_PIPE signal from send when socket is closed
					// if (send(ts.sd, ts.buffer, strlen(ts.buffer), MSG_NOSIGNAL) == -1)
					// {
					// 	if (DEBUG)
					// 		std::cerr << "send failed" << i << std::endl;
					// }
				}
			} 
		} 
	}
	if (DEBUG)
		std::cout << "Main while lopp ended ???? " << std::endl;
}

void	init_server(t_server ts)
{
	ts.opt = TRUE;
	ts.max_clients = 5;
	//set of socket descriptors 

	
	
	//initialise all client_socket[] to 0 so not checked 
	for (int i = 0; i < ts.max_clients; i++) 
	{ 
		ts.client_socket[i] = 0; 
	} 
		
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
	std::cerr << "Usage:" << std::endl
		<< "./ircserv <port> <password>" << std::endl;
	return (1);
}

int	ft_usage_port(void)
{
	std::cerr << "Invalid port:" << std::endl
		<< "port has to be in the range of 1-65535" << std::endl;
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
	t_server	ts;
	ts.port = ok_strtoi(argv[1]);
	if (ts.port <=0 || ts.port > 65535)
		return (ft_usage_port());
	ts.password = argv[2];
	init_server(ts);
	return 0; 
} 
