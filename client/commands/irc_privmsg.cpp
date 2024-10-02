/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:11:13 by tlukanie          #+#    #+#             */
/*   Updated: 2024/10/02 10:12:30 by okraus           ###   ########.fr       */
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
	if (target == tc->botname)
	{
		if (msg->getNick().size())
			target = msg->getNick();
		else
			return ;
	}
	std::string	text = msg->getParams()[1];
	//split text by spaces
	std::vector<std::string>	params;
	std::string					param;
	std::string					action;
	while (text.size())
	{
		param.assign(text, 0, text.find(' '));
		while (text.size() && text[0] != ' ')
			text.erase(0, 1);
		while (text.size() && text[0] == ' ')
			text.erase(0, 1);
		if (action.size())
			params.push_back(param);
		else
			action = param;
		param.clear();
	}
	if (tc->actions.find(action) != tc->actions.end())
	{
		ok_debugger(&(tc->debugger), DEBUG, "Executing action:", action, MYDEBUG);
		//maybe run in try and catch block
		tc->actions[action](tc, target, params);
	}
	else
	{
		ok_debugger(&(tc->debugger), INFO, "Unknown action:", action, MYDEBUG);
		//err_unknowncommand_421(tc, msg_ptr->getSD(), msg_ptr->getCommand());
		//strike count of invalid messages
	}
}