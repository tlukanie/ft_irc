/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic8bot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 13:35:07 by okraus            #+#    #+#             */
/*   Updated: 2024/09/29 15:12:47 by okraus           ###   ########.fr       */
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
	char	buff[512];
	int		rc;
	rc = recv(tc->clientSocket, buff, 512, MSG_NOSIGNAL);
	std::string	message;
	for (int i = 0; i < rc; i++)
	{
		message += buff[i];
		tc->messageIn += buff[i];
	}
	if (rc > 0)
		ok_debugger(&(tc->debugger), NOTICE, "Message was:",  ok_display_buffer(&(tc->debugger), message), MYDEBUG);
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

void	init_client(t_client *tc)
{
	tc->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	// std::cout << "SOCKET: " << tc->clientSocket << std::endl;
	tc->ready = false;
	//Defining Server Address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(tc->port);
	// std::cout << "PORT: " << tc->port << std::endl;
	serverAddress.sin_addr.s_addr = inet_addr(tc->serverIP.c_str()); //inet_pton for IPv6
	// std::cout << "IP: " << tc->serverIP << std::endl;
	//Connecting to the Server // check for error
	connect(tc->clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	//Sending Data to the Server
	// std::string message = "PASS " + tc->password + CRLF "NICK " + tc->botname + CRLF "USER a b c d" CRLF;
	// send(tc->clientSocket, message.c_str(), message.size(), MSG_NOSIGNAL);
	// ok_debugger(&(tc->debugger), INFO, "Message is:", message, MYDEBUG);
	// // std::cout << tc->debugger.log;
	// //Closing the Client Socket
	// char	buff[512];
	// int		rc;
	// rc = recv(tc->clientSocket, buff, 512, MSG_NOSIGNAL);
	// message.clear();
	// for (int i = 0; i < rc; i++)
	// 	message += buff[i];
	// ok_debugger(&(tc->debugger), NOTICE, "Buffer was:", message, MYDEBUG);
	// // std::cout << tc->debugger.log;
	// tc->opt = TRUE;
	// //create a master socket 
	// if( (tc->master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	// {
	// 	ok_debugger(&(tc->debugger), ERROR, "Socket failed", ok_itostr(errno), MYDEBUG);
	// 	//exit(EXIT_FAILURE);
	// 	throw (std::runtime_error("Socket failed")); 
	// } 
	
	// //client startc by listening for new users
	// tc->state = READING_LOOP;

	// //wait for an activity on one of the socketc , timeout is not NULL
	// // so it is non blocking
	// //timeout set for 15 s 0 ms
	// tc->timeout.tv_sec = 15;
	// tc->timeout.tv_usec = 0;

	// //set master socket to allow multiple users , 
	// //this is just a good habit, it will work without this 
	// if( setcockopt(tc->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&tc->opt, 
	// 	sizeof(tc->opt)) < 0 ) 
	// { 
	// 	ok_debugger(&(tc->debugger), ERROR, "Setcockopt failed", ok_itostr(errno), MYDEBUG);
	// 	//exit(EXIT_FAILURE);
	// 	throw (std::runtime_error("Setcockopt failed")); 
	// } 
	
	// //type of socket created 
	// tc->address.sin_family = AF_INET; 
	// tc->address.sin_addr.s_addr = INADDR_ANY; //is it localhost or any IP address on the machine?
	// tc->address.sin_port = htons(tc->port); 
		
	// //bind the socket to localhost port 8888 
	// if (bind(tc->master_socket, (struct sockaddr *)&tc->address, sizeof(tc->address))<0) 
	// { 
	// 	ok_debugger(&(tc->debugger), ERROR, "Bind failed", ok_itostr(errno), MYDEBUG);
	// 	//exit(EXIT_FAILURE);
	// 	throw (std::runtime_error("Bind failed")); 
	// }
	// ok_debugger(&(tc->debugger), INFO, std::string("Listening on port: ") + ok_itostr(tc->port), "", MYDEBUG);
	// //try to specify maximum of 3 pending users for the master socket 
	// if (listen(tc->master_socket, 3) < 0) 
	// { 
	// 	ok_debugger(&(tc->debugger), ERROR, "Listen failed", ok_itostr(errno), MYDEBUG);
	// 	//exit(EXIT_FAILURE);
	// 	throw (std::runtime_error("Listen failed")); 
	// } 
		
	// //accept the incoming connection 
	// tc->addrlen = sizeof(tc->address); 
	// ok_debugger(&(tc->debugger), INFO, "Waiting for users ...", "", MYDEBUG);


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

int main()
{
	t_client	tc;

	signal(SIGINT, signal_handler); 
	irc_init_debugger(&(tc.debugger));
	if (irc_read_client_config(&tc))
		return (1);

	try
	{
		init_client(&tc);
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