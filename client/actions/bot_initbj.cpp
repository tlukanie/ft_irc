/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_initbj.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:57:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/04 15:11:33 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_initbj(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	(void)target;
	std::string	reply;

	tc->bjPlayers.clear();
	tc->messageOut += "WHO " + tc->channelBJ + CRLF;
	tc->ready = true;
}