/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:07 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 15:33:45 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ctype.h>

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_expand_ctx
{
	const char	*input;
	char		**envp;
	char		*result;
	int			last_exit_code;
	int			i;
	int 		j;
}	t_expand_ctx;

typedef struct s_split_vars
{
	int 	i;
	int 	j;
	int 	k;
	int 	m;
	int 	len;
	int 	in_quote;
	int 	inside_quotes;
	char 	quote_char;
	char 	q_char;
	char 	*raw_token;
	char 	*expanded;
	int 	was_single_quoted;
}	t_split_vars;

typedef struct s_pipe_state
{
	int		i;
	int		j;
	int		start;
	int		in_quote;
	char	quote;
	char	**result;
}	t_pipe_state;

// funciones de main
void	init_shell(void);

// funciones parser
t_cmd	*parse_single_command(char *input, char **envp, int last_exit_code);
t_cmd	*parse_input(char *input, char **envp, int last_exit_code);
int		validate_quotes(const char *input);
char	**split_args(char *input, char **envp, int last_exit_code);
// from expand.c
char	*expand_variables(const char *input, char **envp, int last_exit_code);
// from expand_utils.c
void	handle_exit_code(t_expand_ctx *ctx);
void	handle_env_var(t_expand_ctx *ctx);
void 	allocation_error(char *str);
// from env_lookup.c
char	*get_env_value(t_expand_ctx *ctx, int *i);
char	*find_env_value(t_expand_ctx *ctx, const char *var_name);
char	*extract_var_name(t_expand_ctx *ctx, int *i);
// from lexer_expand.c
void	expand_token(t_split_vars *vars, char **envp, int l_e_c);
// from lexer_token.c
void	read_token(t_split_vars *vars, char *input);
int		copy_token(t_split_vars *vars, char *input);
int		init_token(t_split_vars *vars, int start);
// from lexer_utils.c
int	    is_quote(char c);
char 	**free_and_return_null(t_split_vars *vars, char **args);
int		validate_quotes(const char *input);
t_split_vars		*init_vars(void);
// from lexer_parse.c
int		parse_token(t_split_vars *vars, char *input, char **envp, int l_e_c);
char	**split_args(char *input, char **envp, int last_exit_code);


// funciones ejecucción
int		open_redirs(t_cmd *cmd);
int		execute_cmds(t_cmd *cmd, char ***envp);
char	*get_cmd_path(char *cmd, char **envp);
int		ft_cd(char **args, char **envp);
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_env(char **envp);
int		ft_unset(char **args, char ***envp);
int		ft_export(char **args, char ***envp);

// funciones de built-ins
int		is_builtin(char *cmd);
int		is_parent_builtin(const char *cmd);
int		exec_builtin(t_cmd *cmd, char ***envp);

// funciones de señales
void	setup_signals(void);
void	handle_sigint(int sig);

// otros utilitarios
void	free_cmds(t_cmd *cmd);
int		ft_isdigit(int c);
void	free_env(char **env);

#endif
