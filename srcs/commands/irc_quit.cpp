/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:55:41 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:56:11 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// Error
// https://modern.ircdocs.horse/#error-message
// QUIT
// :dan-!d@localhost QUIT :Quit: Bye for now!
// https://modern.ircdocs.horse/#quit-message
void    irc_quit(Message* msg, struct s_server *ts)
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