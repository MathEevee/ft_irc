# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ede-lang <ede-lang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 14:46:49 by ede-lang          #+#    #+#              #
#    Updated: 2024/11/22 13:46:20 by ede-lang         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCDIR		:= ./src
OBJDIR		:= ./obj

NAME	:= ircserv
CC		:= c++
SRCS 	:= main.cpp Channel.cpp Client.cpp Server.cpp ServerCommand.cpp
OBJS	:= $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

CFLAGS	:= -Wextra -Wall -Werror -g -std=c++98


all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@test -d $(OBJDIR) || mkdir $(OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $< && printf "$(GREEN)✔️ $(notdir $<) compiled\n"

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)

$(OBJDIR):
	mkdir -p $@

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, valgrind, run%  