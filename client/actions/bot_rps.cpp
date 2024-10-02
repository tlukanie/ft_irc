/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_rps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:23:32 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 10:23:33 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_rps(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string	result;

	int rng = rand() % 3;
	if (rng & 1)
		result = "Rock!";
	else if (rng & 2)
		result = "Paper!";
	else
		result = "Scissors!";
	tc->messageOut += "NOTICE " + target + " :" + result + CRLF;
	tc->ready = true;
}
