/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:08:41 by okraus            #+#    #+#             */
/*   Updated: 2024/09/28 13:03:30 by okraus           ###   ########.fr       */
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
# include "../../includes/ircserv.hpp"

# define CHANNEL_INVITE	1	// +i
# define CHANNEL_TOPIC	2	// +t
# define CHANNEL_KEY	4	// +k
# define CHANNEL_LIMIT	8	// +l

class Channel
{
	public:
		~Channel(void);
		Channel(std::string channelName, unsigned short sd);

		std::string const	&getChannelName(void);
		std::string const	&getTopic(void);
		std::string const	&getKey(void);
		void				setTopic(std::string const &topic);
		void				setKey(std::string const &key);
		unsigned int		getModeFlags(void);
		void				addModeFlags(unsigned int flag);
		void				removeModeFlags(unsigned int flag);
		int					getChannelLimit(void);
		int					getUsers(void);
		void				setChannelLimit(int channelLimit);
		bool				isOperator(unsigned short sd);
		bool				hasUser(unsigned short sd);
		void				addOperator(unsigned short sd);
		void				removeOperator(unsigned short sd);
		int					addUser(unsigned short sd);
		int					removeUser(unsigned short sd);// returns number of users in the channel
		std::string const	print(bool colour);

	private:
		Channel(void);
		Channel(const Channel& copy);
		Channel &operator	= (const Channel &src);

		
		std::string						_channelName;
		std::string						_topic;
		std::string						_key;	//password
		unsigned int					_modeFlags;
		int								_channelLimit;
		std::map<unsigned short, bool>	_usersInChannel;	//sd : op permission
};

#endif