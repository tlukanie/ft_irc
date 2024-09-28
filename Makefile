# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/30 10:33:19 by okraus            #+#    #+#              #
#    Updated: 2024/09/28 16:17:00 by tlukanie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
SRCS	=	srcs/ircserv.cpp \
			srcs/classes/Channel.cpp \
			srcs/classes/Message.cpp \
			srcs/classes/User.cpp \
			srcs/commands/irc_mode.cpp \
			srcs/commands/irc_cap.cpp \
			srcs/commands/irc_pass.cpp \
			srcs/commands/irc_nick.cpp \
			srcs/commands/irc_user.cpp \
			srcs/commands/irc_ping.cpp \
			srcs/commands/irc_pong.cpp \
			srcs/commands/irc_quit.cpp \
			srcs/commands/irc_join.cpp \
			srcs/commands/irc_part.cpp \
			srcs/commands/irc_topic.cpp \
			srcs/commands/irc_invite.cpp \
			srcs/commands/irc_kick.cpp \
			srcs/commands/irc_away.cpp \
			srcs/commands/irc_privmsg.cpp \
			srcs/commands/irc_notice.cpp \
			srcs/commands/irc_who.cpp \
			srcs/commands/irc_whois.cpp \
			srcs/utils/colours.cpp \
			srcs/utils/debugger.cpp \
			srcs/utils/numeric_replies.cpp \
			srcs/utils/utils.cpp
OBJ_DIR	=	objs/
OBJS	=	$(SRCS:%.cpp=%.o)
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

%.o: %.cpp $(HEADERS)
	$(CPP) $(FLAGS) -c $< -o $@

# objs/%.o: srcs/%.cpp | objdir
# 	$(CPP) $(FLAGS) -c $< -o $@

objdir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJS)
	rm -f $(C_OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(CLIENT)

re: fclean all

.PHONY: all, clean, fclean, re, bonus