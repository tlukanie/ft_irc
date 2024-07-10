/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 14:38:22 by okraus            #+#    #+#             */
/*   Updated: 2024/07/10 15:33:43 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP
# include <stdio.h>
# include <iostream>
# include <sstream>
# include <cstring>
# include <csignal>
# include <string.h> //strlen 
# include <stdlib.h> 
# include <errno.h> 
# include <unistd.h> //close 
# include <arpa/inet.h> //close 
# include <sys/types.h> 
# include <sys/socket.h> 
# include <netinet/in.h> 
# include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
# include <map>
# include <algorithm>

# include "Connection.hpp"

# define TRUE 1 
# define FALSE 0

# ifndef DEBUG
#  define DEBUG 1
# endif

# define MESSAGE "ECHO Daemon v1.0 \r\n"

# define MYENDL "\t\t" << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl

typedef struct s_server {
	std::string password;
	int	port;
	int	opt;
	int	addrlen;
	int	master_socket;
	int	new_socket;
	int	activity;
	int	valread;
	int	sd;
	int	max_sd;
	struct sockaddr_in	address;
	struct timeval		timeout;
	char buffer[1025]; //data buffer of 1K
	std::map<int, Connection*>	connections;
}	t_server;

#endif
