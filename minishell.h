/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:07 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/17 16:45:21 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <string.h>

typedef struct s_lexer_ctx
{
	int 	i;
	int 	j;
	int 	start;
	int		len;
	int 	k;
	int 	m;
	int 	in_quote;
	int 	inside_quotes;
	char 	quote_char;
	char 	q_char;
	char 	*raw_token;
	char 	*expanded;
}	t_lexer_ctx;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_parse_ctx
{
	t_cmd	**head;
	t_cmd	**tail;
	char	**envp;
	int 	code;
}	t_parse_ctx;

// funciones de main
void	init_shell(void);
void	shell_loop(char ***envp);
void	process_input(char *input, char ***envp);

// funciones parser
t_cmd	*parse_single_command(char *input, char **envp, int last_exit_code);
t_cmd	*parse_input(char *input, char **envp, int last_exit_code);
char	**split_args(char *input, char **envp, int last_exit_code);
char	*expand_variables(const char *input, char **envp, int last_exit_code);
char	*get_env_value(const char *var, char **envp);
int		validate_quotes(const char *input);
int		handle_escape(const char *input, t_lexer_ctx *ctx);
int		handle_quote_chars(const char *input, t_lexer_ctx *ctx);
int		update_quote_status(char c, t_lexer_ctx *ctx);
void	init_lexer_ctx(t_lexer_ctx *ctx);
int		parse_token_boundaries(const char *input, t_lexer_ctx *ctx);
char	**split_by_pipe(char *input);
int		is_quote(char c);

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
