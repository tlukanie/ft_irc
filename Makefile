# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/30 10:33:19 by okraus            #+#    #+#              #
#    Updated: 2024/10/07 16:42:11 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER		=	ircserv
SRVDIR		=	server/
CLNTDIR		=	client/
#VPAT	H	=	srcs:srcs/classes:srcs/commands:srcs/utils
SRCS		=	ircserv.cpp \
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
				utils/irc_init_debugger.cpp \
				utils/numeric_replies.cpp \
				utils/utils.cpp
OBJDIR		=	objs/
OBJDIRS		=	$(OBJDIR)server/
OBJDIRC		=	$(OBJDIR)client/
OBJS		=	$(SRCS:%.cpp=$(OBJDIRS)%.o)
HEADERS		=	includes/ircserv.hpp \
				includes/enums.hpp \
				server/classes/Channel.hpp \
				server/classes/Message.hpp \
				server/classes/User.hpp
CLIENT		=	magic8bot
CSRCS		=	client/magic8bot.cpp \
				actions/bot_blackjack.cpp \
				actions/bot_card.cpp \
				actions/bot_date.cpp \
				actions/bot_flip.cpp \
				actions/bot_hit.cpp \
				actions/bot_initbj.cpp \
				actions/bot_q.cpp \
				actions/bot_roll.cpp \
				actions/bot_rps.cpp \
				actions/bot_stand.cpp \
				actions/bot_time.cpp \
				actions/bot_utime.cpp \
				classes/BlackJack.cpp \
				classes/CardDeck.cpp \
				classes/CardPlayer.cpp \
				classes/Message.cpp \
				commands/irc_315.cpp \
				commands/irc_352.cpp \
				commands/irc_privmsg.cpp \
				utils/bj_end.cpp \
				utils/debugger.cpp \
				utils/irc_init_debugger.cpp \
				utils/irc_read_client_config.cpp \
				utils/utils.cpp
COBJS		=	$(CSRCS:%.cpp=$(OBJDIRC)%.o)
CHEADERS	=	includes/magic8bot.hpp \
				includes/enums.hpp \
				client/classes/Message.hpp
CPP			=	c++
FLAGS		=	-g -Wall -Wextra -Werror -std=c++98
NAME		=	$(SERVER) $(CLIENT)

all:			$(NAME)

bonus:			$(CLIENT)

$(SERVER):		$(OBJS)
				$(CPP) $(FLAGS) $(OBJS) -o $(SERVER)

$(CLIENT):		$(COBJS)
				$(CPP) $(FLAGS) $(COBJS) -o $(CLIENT)

$(OBJDIRS)%.o:	$(SRVDIR)%.cpp $(HEADERS)
				@mkdir -p $(@D)
				$(CPP) $(FLAGS) -c $< -o $@

$(OBJDIRC)%.o:	$(CLNTDIR)%.cpp $(CHEADERS)
				@mkdir -p $(@D)
				$(CPP) $(FLAGS) -c $< -o $@

# %.o: %.cpp $(HEADERS)
# 	$(CPP) $(FLAGS) -c $< -o $@
# objs/%.o: srcs/%.cpp | objdir
# 	$(CPP) $(FLAGS) -c $< -o $@

# objdir:
# 	mkdir -p $(OBJDIR)

clean:
				rm -rf $(OBJDIR)

fclean: clean
				rm -f $(NAME)
				rm -f $(CLIENT)

re:				fclean all

.PHONY: all, clean, fclean, re, bonus