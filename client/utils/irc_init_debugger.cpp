/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_init debugger.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 10:24:07 by okraus            #+#    #+#             */
/*   Updated: 2024/09/29 10:28:11 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/magic8bot.hpp"

void	irc_init_debugger(s_debugger *debugger)
{
	debugger->date = false;
	debugger->time = true;
	debugger->utime = true;
	debugger->precision = 4; // should be 0-6
	debugger->colour = true;
	debugger->extra = true;
	debugger->fd = 1;
}
