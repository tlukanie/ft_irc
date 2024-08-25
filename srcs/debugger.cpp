/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:07:48 by okraus            #+#    #+#             */
/*   Updated: 2024/08/25 16:29:29 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debugger.hpp"

void	ok_debugger(int debuglvl, int msglvl, std::string message)
{
	if (debuglvl <= msglvl && msglvl == 0)
	{
		std::cerr << GREYBG_COLOUR "DEBUG:" NO_COLOUR "   " GREY_COLOUR
			<< message << NO_COLOUR << std::endl;
	}
	else if (debuglvl <= msglvl && msglvl == 1)
	{
		std::cerr << GREENBG_COLOUR "INFO:" NO_COLOUR "    " GREEN_COLOUR
			<< message << NO_COLOUR << std::endl;
	}
	else if (debuglvl <= msglvl && msglvl == 2)
	{
		std::cerr << YELLOWBG_COLOUR "NOTICE:" NO_COLOUR "  " YELLOW_COLOUR
			<< message << NO_COLOUR << std::endl;
	}
	else if (debuglvl <= msglvl && msglvl == 3)
	{
		std::cerr << ORANGEBG_COLOUR "WARNING:" NO_COLOUR " " ORANGE_COLOUR
			<< message << NO_COLOUR << std::endl;
	}
	else if (debuglvl <= msglvl && msglvl == 4)
	{
		std::cerr << ERROR_COLOUR "ERROR:" NO_COLOUR "   " RED_COLOUR
			<< message << NO_COLOUR << std::endl;
	}
}