# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/30 10:33:19 by okraus            #+#    #+#              #
#    Updated: 2024/06/30 11:07:02 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
SRC_DIR	=	srcs/
FILES	=	server.cpp
SRCS	=	$(addprefix $(SRC_DIR), $(FILES))
OBJ_DIR	=	objs/
OBJS	=	$(addprefix $(OBJ_DIR), $(FILES:%.cpp=%.o))
CLIENT	=	client
C_SRCS	=	TestingClient1.cpp
C_OBJS	=	TestingClient1.o
CPP		=	c++
FLAGS	=	-g -Wall -Wextra -Werror -std=c++98

all: $(NAME) $(CLIENT)

$(NAME): $(OBJS)
	$(CPP) $(FLAGS) $(OBJS) -o $(NAME)

$(CLIENT): $(C_OBJS)
	$(CPP) $(FLAGS) $(C_OBJS) -o $(CLIENT)

%.o: %.cpp
	$(CPP) $(FLAGS) -c $< -o $@

objs/%.o: srcs/%.cpp | objdir
	$(CPP) $(FLAGS) -c $< -o $@

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