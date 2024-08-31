/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:07:48 by okraus            #+#    #+#             */
/*   Updated: 2024/08/31 15:00:13 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debugger.hpp"

void	ok_debugger(DebugLvl globalLevel, DebugLvl debugLevel, std::string message, std::string details, std::string extra)
{
	if (globalLevel <= debugLevel && debugLevel == DEBUG)
	{
		std::cout << GREYBG_COLOUR "DEBUG:" NO_COLOUR "   " GREY_COLOUR;
	}
	else if (globalLevel <= debugLevel && debugLevel == INFO)
	{
		std::cout << GREENBG_COLOUR "INFO:" NO_COLOUR "    " GREEN_COLOUR;
	}
	else if (globalLevel <= debugLevel && debugLevel == NOTICE)
	{
		std::cout << YELLOWBG_COLOUR "NOTICE:" NO_COLOUR "  " YELLOW_COLOUR;
	}
	else if (globalLevel <= debugLevel && debugLevel == WARNING)
	{
		std::cout << ORANGEBG_COLOUR "WARNING:" NO_COLOUR " " ORANGE_COLOUR;
	}
	else if (globalLevel <= debugLevel && debugLevel == ERROR)
	{
		std::cout << ERROR_COLOUR "ERROR:" NO_COLOUR "   " RED_COLOUR;
	}
	std::cout << message << " " << details;
	if (globalLevel == EXTRADEBUG)
		std::cout << "\t\t" << extra;
	std::cout << NO_COLOUR << std::endl;
}