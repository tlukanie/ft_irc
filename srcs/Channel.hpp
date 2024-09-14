/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/09/14 12:28:23 by okraus           ###   ########.fr       */
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

# define CHANNEL_INVITE	1	// +i
# define CHANNEL_TOPIC	2	// +t
# define CHANNEL_KEY	4	// +k
# define CHANNEL_LIMIT	8	// +l

class Channel
{
	public:
		Channel(void);
		Channel(const Channel& copy);
		Channel &operator	= (const Channel &src);
		~Channel(void);

		Channel(std::string channelName);
		std::string		getChannelName(void);
		std::string		getTopic(void);
		void			setTopic(std::string topic);
		std::string		getKey(void);
		void			setKey(std::string key);
		unsigned int	getModeFlags(void);
		void			addModeFlags(unsigned int flag);
		void			removeModeFlags(unsigned int flag);
		int				getChannelLimit(void);
		void			setChannelLimit(int channelLimit);

	private:
		std::string		_channelName;
		std::string		_topic;
		std::string		_key;	//password
		unsigned int	_modeFlags;
		int				_channelLimit;
};

#endif