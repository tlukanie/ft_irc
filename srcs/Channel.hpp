/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/09/21 10:57:46 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

//# include <cstddef>
# include <iostream>
# include <vector>
# include <map>
# include <stdint.h>
// # include <fstream>
// # include <sstream>
// # include <ctime>
// # include <cstring>

# include "Message.hpp"
# include "colours.hpp"
# include "utils.hpp"

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

		Channel(std::string channelName, unsigned short sd);
		std::string		getChannelName(void);
		std::string		getTopic(void);
		void			setTopic(std::string topic);
		std::string		getKey(void);
		void			setKey(std::string key);
		unsigned int	getModeFlags(void);
		void			addModeFlags(unsigned int flag);
		void			removeModeFlags(unsigned int flag);
		int				getChannelLimit(void);
		int				getUsers(void);
		void			setChannelLimit(int channelLimit);
		bool			isOperator(unsigned short sd);
		bool			hasUser(unsigned short sd);
		void			addOperator(unsigned short sd);
		void			removeOperator(unsigned short sd);
		int				addUser(unsigned short sd);
		int				removeUser(unsigned short sd);// returns number of users in the channel
		std::string		print(bool colour);

	private:
		std::string						_channelName;
		std::string						_topic;
		std::string						_key;	//password
		unsigned int					_modeFlags;
		int								_channelLimit;
		std::map<unsigned short, bool>	_usersInChannel;	//sd : op permission
};

#endif