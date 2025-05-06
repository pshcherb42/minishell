# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akreise <akreise@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 17:13:20 by pshcherb          #+#    #+#              #
#    Updated: 2025/05/03 17:38:08 by akreise          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT = libft/libft.a

SRCS = \
	main.c \
	parser/parser.c \
	parser/cmd_utils.c \
	parser/split_pipe.c \
	parser/tokenizer.c \
	parser/redirection.c \
	parser/parser_state.c \
	parser/expand.c \
	parser/expand_utils.c \
	parser/env_lookup.c \
	parser/lexer_expand.c \
	parser/lexer_parse.c \
	parser/lexer_token.c \
	parser/lexer_utils.c \
	executor/builtin_utils.c \
	executor/ft_cd.c \
	executor/ft_echo.c \
	executor/ft_env.c \
	executor/ft_exit.c \
	executor/ft_export.c \
	executor/ft_get_cmd_path.c \
	executor/ft_pwd.c \
	executor/ft_unset.c \
	executor/exec.c \
	executor/open_redirs.c \
	executor/exec_child.c \
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