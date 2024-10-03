/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_q.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:23:37 by okraus            #+#    #+#             */
/*   Updated: 2024/10/03 11:37:16 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_q(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string	result;

	result = tc->answers[rand() % 20];
	tc->messageOut += "NOTICE " + target + " :" + result + CRLF;
	tc->ready = true;
}
