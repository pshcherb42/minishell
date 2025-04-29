# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 17:13:20 by pshcherb          #+#    #+#              #
#    Updated: 2025/04/29 16:44:41 by pshcherb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT = libft/libft.a

SRCS = \
	main.c \
	parser/lexer.c \
	parser/parser.c \
	parser/expand.c \
	parser/expand_utils.c \
	parser/env_lookup.c \
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

$(NAME): $(OBJS) Makefile $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
		make -C libft

all: $(NAME)

clean:
	rm -f $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re 