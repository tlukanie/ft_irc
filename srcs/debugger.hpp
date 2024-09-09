/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:08:26 by okraus            #+#    #+#             */
/*   Updated: 2024/09/09 15:43:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef DEBUGGER_HPP
# define DEBUGGER_HPP
# include <iostream>
# include <sstream>
# include <stdint.h>
# include <ctime>
# include <sys/time.h>
# include <iomanip>
# include "server.hpp"

# define ERROR_BLINKING_COLOUR "\033[1;5;38:5:226;48:5:160m"
# define ERROR_COLOUR "\033[1;38:5:226;48:5:160m"
# define REDBG_COLOUR "\033[1;38:5:231;48:5:160m"
# define GREENBG_COLOUR "\033[1;38:5:0;48:5:34m"
# define BLUEBG_COLOUR "\033[1;38:5:231;48:5:33m"
# define CYANBG_COLOUR "\033[1;38:5:0;48:5:51m"
# define MAGENTABG_COLOUR "\033[1;38:5:231;48:5:201m"
# define YELLOWBG_COLOUR "\033[1;38:5:0;48:5:226m"
# define ORANGEBG_COLOUR "\033[1;38:5:0;48:5:214m"
# define WHITEBG_COLOUR "\033[1;38:5:0;48:5:231m"
# define GREYBG_COLOUR "\033[1;38:5:0;48:5:246m"
# define BLACKBG_COLOUR "\033[1;38:5:231;48:5:0m"
# define RED_COLOUR "\033[1;38:5:160m"
# define GREEN_COLOUR "\033[1;38:5:34m"
# define BLUE_COLOUR "\033[1;38:5:33m"
# define CYAN_COLOUR "\033[1;38:5:51m"
# define MAGENTA_COLOUR "\033[1;38:5:201m"
# define YELLOW_COLOUR "\033[1;38:5:226m"
# define ORANGE_COLOUR "\033[1;38:5:214m"
# define WHITE_COLOUR "\033[1;38:5:231m"
# define GREY_COLOUR "\033[1;38:5:246m"
# define BLACK_COLOUR "\033[1;38:5:0m"
# define NO_COLOUR "\033[0m"

struct s_debugger;

//change int i to enum
void		ok_debugger(s_debugger *debugger, DebugLvl globalLevel, std::string message, std::string details, std::string extra);
std::string	ok_display_reply(s_debugger *debugger, std::string reply);

#endif
