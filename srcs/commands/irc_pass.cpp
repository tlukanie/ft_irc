/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:45:53 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:46:44 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// PASS a
// https://modern.ircdocs.horse/#pass-message
// Numeric replies:
void    irc_pass(Message* msg, struct s_server *ts)
{
	std::string reply;

	if (!msg->getParams().size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	// ERR_ALREADYREGISTERED (462)
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		err_alreadyregistered_462(ts, msg->getSD());
		return ;
	}
	if (msg->getParams()[0] == ts->password)
	{
		ts->users[msg->getSD()]->addAuthFlag(PASSWORD);
	}
	else
	{
		// ERR_PASSWDMISMATCH (464)
		err_passwdmismatch_464(ts, msg->getSD());
		// possibly count number of incorrect tries
	}
	
}