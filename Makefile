# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/06 19:15:22 by pshcherb          #+#    #+#              #
#    Updated: 2025/06/08 16:55:33 by pshcherb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

LIBFT = libft/libft.a

SRCS = \
	main/main.c \
	main/init.c \
	main/env_utils.c \
	main/input.c \
	main/shell_loop.c \
	main/env_list.c \
	main/env_list_to_array.c \
	main/env_list_utils.c \
	parser/parser.c \
	parser/parser_utils.c \
	parser/cmd_utils.c \
	parser/split_pipe.c \
	parser/tokenizer.c \
	parser/process_token.c \
	parser/redirection.c \
	parser/parser_state.c \
	parser/expand.c \
	parser/expand_utils.c \
	parser/env_lookup.c \
	parser/lexer_expand.c \
	parser/lexer_init.c \
	parser/lexer_copy.c \
	parser/lexer_parse.c \
	parser/split_args.c \
	parser/find_invalid_token.c \
	parser/process_segment.c \
	parser/check_invalid_op_seq.c \
	parser/split_pipe_utils.c \
	parser/lexer_token.c \
	parser/lexer_utils.c \
	parser/redirection_heredoc.c \
	executor/builtin_utils.c \
	executor/ft_cd.c \
	executor/cd_utils.c \
	executor/cd_utils_2.c \
	executor/cd_utils_3.c \
	executor/ft_echo.c \
	executor/ft_env.c \
	executor/ft_exit.c \
	executor/ft_export.c \
	executor/export_utils.c \
	executor/export_add_eq.c \
	executor/export_utils_2.c \
	executor/ft_get_cmd_path.c \
	executor/ft_pwd.c \
	executor/ft_unset.c \
	executor/exec.c \
	executor/exec_pipeline.c \
	executor/pipeline_utils.c \
	executor/open_redirs.c \
	executor/exec_child.c \
	executor/child_utils.c \
	executor/builtin.c \
	executor/exec_utils.c \
	signals/signals.c \
	redirection/heredoc.c \
	redirection/heredoc_utils.c \
	redirection/tmp_heredoc.c \
	utils/utils.c \
	utils/utils2.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

$(NAME): $(OBJS) Makefile minishell.h $(LIBFT)
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
