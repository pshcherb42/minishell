# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 17:13:20 by pshcherb          #+#    #+#              #
#    Updated: 2025/04/14 17:13:22 by pshcherb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

SRCS = \
	main.c \
	parser/lexer.c \
	parser/parser.c \
	parser/expander.c \
	executor/exec.c \
	executor/builtin.c \
	executor/exec_utils.c \
	signals/signals.c \
	redirection/redir.c \
	env/env.c \
	env/export.c \
	utils/utils.c 

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

$(NAME): $(OBJS) Makefile #libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 