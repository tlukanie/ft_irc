/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:51:19 by tlukanie          #+#    #+#             */
/*   Updated: 2024/09/28 15:23:32 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

// USER net net localhost :net
// USER guest 0 * :Ronnie Reagan
//Parameters: <username> <hostname> <servername> <realname> //hostname and servername can be ignored for local users
// https://www.rfc-editor.org/rfc/rfc2812#section-3.1.3
// Parameters: <user> <mode> <unused> <realname>
//      Command: USER
//   Parameters: <username> 0 * <realname>
// https://modern.ircdocs.horse/#user-message
// reply :IRCQ+ 001 net :Welcome to IRCQ+ net!net@127.0.0.1
//net!net@127.0.0.1 <<<< save this string somehow? Create function to make it?
//Numeric Replies:

// ERR_NEEDMOREPARAMS (461)
// ERR_ALREADYREGISTERED (462)
void	irc_user(Message* msg, struct s_server *ts)
{
	if (!(ts->users[msg->getSD()]->getAuthFlags() & PASSWORD))
		return ;
	// we need to check that there are 4 parameters?
	if (msg->getParams().size() != 4 || !msg->getParams()[0].size())
	{
		err_needmoreparams_461(ts, msg->getSD(), msg->getCommand());
		return ;
	}
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		err_alreadyregistered_462(ts, msg->getSD());
		return ;
	}
	ts->users[msg->getSD()]->setUserName(msg->getParams()[0]);
	ts->users[msg->getSD()]->setHostName(msg->getParams()[1]);
	ts->users[msg->getSD()]->setServerName(msg->getParams()[2]);
	ts->users[msg->getSD()]->setRealName(msg->getParams()[3]);
	// process the 4 parameters
	ts->users[msg->getSD()]->addAuthFlag(USER);
	if (ts->users[msg->getSD()]->getAuthFlag())
	{
		rpl_welcome_001(ts, msg->getSD());
	}
}