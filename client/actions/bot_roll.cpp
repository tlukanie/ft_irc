/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_roll.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:23:37 by okraus            #+#    #+#             */
/*   Updated: 2024/10/02 10:32:36 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	bot_roll(t_client *tc, std::string target, std::vector<std::string> params)
{
	(void)params;
	std::string	result;
	int			n;

	if (params.size() && params[0].size())
	{
		if (params[0][0] == 'd')
			n = ok_strtoi<int>(params[0].substr(1));
		else
			n = ok_strtoi<int>(params[0]);
	}
	else
		n = 6;
	if (n < 2 || n > 1024)
		result = "invalid input";
	else
		result = ok_itostr(rand() % n + 1);
	tc->messageOut += "NOTICE " + target + " :" + result + CRLF;
	tc->ready = true;
}
