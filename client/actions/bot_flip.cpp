/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_flip.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 10:11:11 by okraus            #+#    #+#             */
/*   Updated: 2024/10/01 10:21:24 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

//PRIVMSG
//https://modern.ircdocs.horse/#privmsg-message
//:user123!net@127.0.0.1 PRIVMSG #aaa :hello there
//not sending messages to chanops only, yet...

void	bot_flip(t_client *tc, std::string target)
{
	std::string	result;

	if (rand() % 2)
		result = "Heads!";
	else
		result = "Tails!";
	tc->messageOut += "NOTICE " + target + " :" + result + CRLF;
	tc->ready = true;
}
