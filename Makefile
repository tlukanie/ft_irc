# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/30 10:33:19 by okraus            #+#    #+#              #
#    Updated: 2024/09/28 15:55:29 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
SRCDIR	=	srcs/
#VPATH	=	srcs:srcs/classes:srcs/commands:srcs/utils
SRCS	=	ircserv.cpp \
			classes/Channel.cpp \
			classes/Message.cpp \
			classes/User.cpp \
			commands/irc_away.cpp \
			commands/irc_cap.cpp \
			commands/irc_invite.cpp \
			commands/irc_join.cpp \
			commands/irc_kick.cpp \
			commands/irc_mode.cpp \
			commands/irc_nick.cpp \
			commands/irc_notice.cpp \
			commands/irc_part.cpp \
			commands/irc_pass.cpp \
			commands/irc_ping.cpp \
			commands/irc_pong.cpp \
			commands/irc_privmsg.cpp \
			commands/irc_quit.cpp \
			commands/irc_topic.cpp \
			commands/irc_user.cpp \
			commands/irc_who.cpp \
			commands/irc_whois.cpp \
			utils/colours.cpp \
			utils/debugger.cpp \
			utils/numeric_replies.cpp \
			utils/utils.cpp
OBJDIR	=	objs/
OBJS	=	$(SRCS:%.cpp=$(OBJDIR)%.o)
CLIENT	=	client
C_SRCS	=	TestingClient1.cpp
C_OBJS	=	TestingClient1.o
HEADERS	=	includes/ircserv.hpp \
			includes/enums.hpp \
			srcs/classes/Channel.hpp \
			srcs/classes/Message.hpp \
			srcs/classes/User.hpp
CPP		=	c++
FLAGS	=	-g -Wall -Wextra -Werror -std=c++98

all: $(NAME) $(CLIENT)

$(NAME): $(OBJS)
	$(CPP) $(FLAGS) $(OBJS) -o $(NAME)

$(CLIENT): $(C_OBJS)
	$(CPP) $(FLAGS) $(C_OBJS) -o $(CLIENT)

$(OBJDIR)%.o: $(SRCDIR)%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CPP) $(FLAGS) -c $< -o $@

%.o: %.cpp $(HEADERS)
	$(CPP) $(FLAGS) -c $< -o $@
# objs/%.o: srcs/%.cpp | objdir
# 	$(CPP) $(FLAGS) -c $< -o $@

objdir:
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS)
	rm -f $(C_OBJS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(CLIENT)

re: fclean all

.PHONY: all, clean, fclean, re, bonus