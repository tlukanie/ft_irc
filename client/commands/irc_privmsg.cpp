/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:11:13 by tlukanie          #+#    #+#             */
/*   Updated: 2024/10/01 10:28:34 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there
//not sending messages to chanops only, yet...

void	irc_privmsg(Message* msg, struct s_client *tc)
{
	std::string	reply;

	if (!msg->getParams().size())
	{
		// err_norecipient_411(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	if (msg->getParams().size() < 2)
	{
		// err_notexttosend_412(ts, msg->getSD());
		return ;
	}
	std::string	target = msg->getParams()[0];
	//if target is not channel (it is bot itself),
	//target from prefix
	std::string	text = msg->getParams()[1];
	//split text by spaces

	if (tc->actions.find(text) != tc->actions.end())
	{
		ok_debugger(&(tc->debugger), DEBUG, "Executing action:", text, MYDEBUG);
		//maybe run in try and catch block
		tc->actions[text](tc, target);
	}
	else
	{
		ok_debugger(&(tc->debugger), INFO, "Unknown command:", text, MYDEBUG);
		//err_unknowncommand_421(tc, msg_ptr->getSD(), msg_ptr->getCommand());
		//strike count of invalid messages
	}
}