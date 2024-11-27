# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 14:46:49 by ede-lang          #+#    #+#              #
#    Updated: 2024/11/27 14:32:00 by ede-lang         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCDIR		:= ./src
SRCDIRB		:= ./src_bonus
OBJDIR		:= ./obj
OBJDIRB		:= ./objb

NAME	:= ircserv
NAME_B	:= ircserv_bonus
CC		:= c++
SRCS 	:= main.cpp Channel.cpp Client.cpp Server.cpp ServerCommand.cpp
SRCS_B 	:= main.cpp Channel.cpp Client.cpp Server.cpp ServerCommand.cpp
OBJS	:= $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
OBJSB	:= $(addprefix $(OBJDIRB)/, $(SRCS_B:.cpp=.o))

CFLAGS	:= -Wextra -Wall -Werror -g -std=c++98


all: $(NAME)

bonus : $(OBJSB)
	@$(CC) $(OBJSB) -o $(NAME_B)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@test -d $(OBJDIR) || mkdir $(OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $< && printf "$(GREEN)✔️ $(notdir $<) compiled\n"

$(OBJDIRB)/%.o: $(SRCDIRB)/%.cpp
	@test -d $(OBJDIRB) || mkdir $(OBJDIRB)
	@$(CC) $(CFLAGS) -o $@ -c $< && printf "$(GREEN)✔️ $(notdir $<) compiled\n"

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJDIR)
	@rm -rf $(OBJDIRB)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(NAME_B)

re: clean all

.PHONY: all, clean, fclean, re, bonus 
