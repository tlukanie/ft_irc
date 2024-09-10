/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/09/10 15:00:04 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

//# include <cstddef>
# include <iostream>
# include <vector>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>

# include "Message.hpp"
# include "colours.hpp"

class Channel
{
	public:
		Channel(void);
		Channel(const Channel& copy);
		Channel &operator	= (const Channel &src);
		~Channel(void);

		Channel(std::string channelName);
		std::string		getChannelName(void);

	private:
		std::string		_channelName;
};

#endif