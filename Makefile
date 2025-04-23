# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 17:13:20 by pshcherb          #+#    #+#              #
#    Updated: 2025/04/23 14:14:16 by pshcherb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT = Libft/libft.a

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

$(NAME): $(OBJS) Makefile $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
		make -C Libft

all: $(NAME)

clean:
	rm -f $(OBJS)
	make clean -C Libft

fclean: clean
	rm -f $(NAME)
	make fclean -C Libft

re: fclean all

.PHONY: all clean fclean re 