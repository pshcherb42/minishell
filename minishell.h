/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:07 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/09 15:02:50 by akreise          ###   ########.fr       */
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
# include "env_list.h"

# define ARGS_INIT_CAPACITY 10

extern volatile sig_atomic_t	g_sigquit_flag;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	int				heredoc_fd; // nuevo campo
	char			*heredoc_file;
	int				heredoc_interrupted;
	struct s_cmd	*next;
	int				capacity;
}	t_cmd;

typedef struct s_expand_state
{
	char		*result;
	const char	*input;
	int			i;
	int			j;
	int			in_quotes;
	char		quote_char;
}	t_expand_state;

typedef struct s_expand_ctx
{
	const char	*input;
	t_env		*envp;
	char		*result;
	int			last_exit_code;
	int			i;
	int			j;
}	t_expand_ctx;

typedef struct s_split_vars
{
	int		i;
	int		j;
	int		k;
	int		m;
	int		len;
	int		in_quote;
	int		inside_quotes;
	char	quote_char;
	char	q_char;
	char	*raw_token;
	char	*expanded;
	int		was_single_quoted;
	int		syntax_error;
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

typedef struct s_parse_state
{
	char	**segments;
	t_cmd	*head;
	t_cmd	*tail;
	int		i;
	t_env	*env;
	int		last_exit_code;
}	t_parse_state;

typedef struct s_token_state
{
	char			**args;
	t_split_vars	*vars;
	int				cap;
	char			*input;
	t_env			*envp;
	int				lec;
}	t_token_state;

// funciones de main
// from init.c
void			init_shell(t_env **envp);
void			update_shlvl(t_env **envp);
// from env_utils.c
void			set_env_value(const char *value, char ***envp);
char			*find_env_var(t_env *envp, const char *name);
char			**dup_env(char **envp);
char			**create_minimal_env(void);
// from input.c
char			*get_valid_input(void);
// from shell_loop.c
int				process_input(char *input, t_env **envp, int last_exit_code);
void			run_shell_loop(t_env **envp);

// funciones parser
t_cmd			*parse_single_command(char *input, t_env *envp, int l_e_c);
t_cmd			*parse_input(char *input, t_env *envp, int last_exit_code);
// from parser_utils.c
//char			*read_continued_input(const char *input);
int				is_trailing_pipe(const char *input);
// from parser_state.c
int				is_empty_or_spaces(const char *str);
t_cmd			*build_command_list(t_parse_state *state);
int				count_consecutive_operators(const char *str, char op);
int				is_invalid_syntax(const char *segment);
// from check_invalid_op_seq.c
int				check_invalid_op_seq(const char *segment);
// from process_segment.c
int				process_segment(t_parse_state *state);
// from find_invalid_token.c
char			*find_invalid_token(const char *segment);
// from redirection.c
int				handle_redirection(t_cmd *cmd, char **tokens, int i);
// from redirection_heredoc.c
int				handle_input_redirect(t_cmd *cmd, char **tokens, int i);
// from tokenizer.c
t_cmd			*init_and_tokenize(char *in, t_env *env, int lec, char ***tkn);
int				add_arg(t_cmd *cmd, char *token, int *j);
void			cleanup_args_on_error(t_cmd *cmd, int j);
void			fill_cmd_from_tokens(t_cmd *cmd, char **tokens);
// from process_token.c
int				process_token(t_cmd *cmd, char **tokens, int *i, int *j);
// from split_pipe.c
char			**split_by_pipe(char *input);
void			finalize_result(char *input, t_pipe_state *state);
void			process_pipe_found(char *input, t_pipe_state *state);
char			*apply_quote_prefix_if_needed(char *segment, int in_quote);
// from split_pipe_utils.c
void			handle_quote(char c, char *quote, int *in_quote);
char			*extract_segment(char *input, int start, int end);
void			store_segment(t_pipe_state *state, char *segment);
// from cmd_utils.c
t_cmd			*init_cmd(void);
char			**grow_args_array(char **old_args, int old_size, int *capacity);
void			cleanup_cmd_heredocs(t_cmd *cmd);
// from expand.c
char			*expand_variables(const char *input, t_env *envp, int lec);
// from expand_utils.c
void			handle_exit_code(t_expand_ctx *ctx);
void			handle_env_var(t_expand_ctx *ctx);
void			allocation_error(char *str);
// from env_lookup.c
char			*get_env_value(t_expand_ctx *ctx, int *i);
char			*find_env_value(t_expand_ctx *ctx, const char *var_name);
char			*extract_var_name(t_expand_ctx *ctx, int *i);
// funciones lexer
// from lexer_expand.c
void			expand_token(t_split_vars *vars, t_env *envp, int l_e_c);
// from lexer_copy.c
int				copy_token(t_split_vars *vars, char *input);
int				handle_quote_char(t_split_vars *vars, char c);
// from lexer_token.c
void			read_token(t_split_vars *vars, char *input, int start);
// from lexer_init.c
int				init_token(t_split_vars *vars, int start);
// from lexer_utils.c
int				is_quote(char c);
char			**free_and_return_null(t_split_vars *vars, char **args);
int				validate_quotes(const char *input);
t_split_vars	*init_vars(void);
// from lexer_parse.c
int				is_token_quoted(char *input, int pos);
int				parse_and_store_token(t_token_state *state);
int				parse_token(t_split_vars *vars, char *in, t_env *env, int lec);
char			**split_args(char *input, t_env *envp, int last_exit_code);
void			cleanup_partial_args(char **args, int count);
// from split_args.c
int				init_state(t_token_state *state, char *in, t_env *env, int lec);
int				handle_parse_error(t_token_state *state, int ret);
void			mark_quoted_token(t_token_state *state, int was_quoted);
int				process_single_token(t_token_state *state);
char			**finalize_token_array(t_token_state *state);
// funciones ejecucción
// from builtin_utils.c
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_atoi(const char *nptr);
void			ft_pstr(int fd, const char *str);
size_t			ft_strspn(const char *s, const char *accept);
// from builtin.c
int				is_builtin(char *cmd);
int				is_parent_builtin(const char *cmd);
int				exec_builtin(t_cmd *cmd, t_env **env);
// from exec.c
int				execute_cmds(t_cmd *cmd, t_env **envp, int last_exit_code);
pid_t			execute_single_cmd(t_cmd *cmd, t_env **env, int *prev_fd);
// from exec_child.c
void			run_child(t_cmd *cmd, int prev_fd, int pipefd[2], t_env **env);
void			handle_child_exit( int status);
// from child_utils.c
void			exec_child_cmd(t_cmd *cmd, t_env **env);
// from exec_pipeline.c
int				count_commands(t_cmd *cmd);
int				execute_builtin_in_pipeline(t_cmd *current, t_env **env,
					int cmd_count);
int				execute_pipeline(t_cmd *cmd, t_env **env, int last_exit_code);
// from pipeline_utils.c
int				execute_commands_loop(t_cmd *cmd, t_env **env, pid_t *pids);
// from exec_utils.c
char			*join_path(const char *dir, const char *cmd);
void			free_split(char **arr);
char			**split_path(const char *path);
// from open_redirs.c
int				open_redirs(t_cmd *cmd);
// from get_cmd_path.c
char			*get_cmd_path(char *cmd, char **envp);
// from ft_cd.c
int				ft_cd(char **args, t_env **envp);
char			*get_current_dir(void);
// from cd_utils.c
void			replace_env(const char *var_name, const char *value,
					char **envp);
char			*get_target_path(char **args, t_env **env);
char			*join_env_entry(const char *var_name, const char *value);
// from cd_utils_2.c
char			*find_env_var_local(char **envp, const char *name);
void			replace_env(const char *var_name, const char *value,
					char **envp);
void			update_env_vars(char *oldpwd, t_env **envp);
char			*get_target_path_env(char **args, t_env *env);
// from cd_utils_3.c
char			*handle_special_args(char **args, t_env **envp,
					int *error_flag);
char			*handle_special_args_env(char **args, t_env *env,
					int *error_flag);
// from ft_echo.c
int				ft_echo(char **args);
// from ft_env.c
char			*ft_strchr(const char *s, int c);
int				ft_env(char **args, t_env *env);
// from ft_exit.c
int				ft_exit(char **args, int last_exit_code);
int				ft_exit_child(char **args);
// from ft_pwd.c
int				ft_pwd(void);
// from ft_unset.c
int				ft_unset(char **args, t_env **env);
// from ft_export.c
int				ft_export(char **args, t_env **env);
// from export_utils.c
void			print_all_env(t_env *env);
int				add_env_var(char ***envp, char *arg);
int				replace_new_var(char ***envp, char *arg, int name_len);
// from export_utils_2.c
char			*prep_joined(const char *arg, const char *var_name);
int				add_if_needed(t_env **envp, char *arg);
char			*ft_strjoin_free(char *s1, const char *s2);
int				var_exists(char **envp, const char *var_name);
int				is_valid(const char *str);
// from export_add_eq.c
int				ft_add_eq(t_env **env, char *arg, int name_len);
// signals.c
void			setup_signals(void);
void			handle_sigint(int sig);
void			heredoc_sigint(int sig);
// from heredoc.c
void			handle_here_child(const char *delimiter, const char *filename);
int				handle_here_father(pid_t pid, int *status, char **temp_file);
int				read_heredoc_lines(const char *delimiter, const char *filename);
// from heredoc_utils.c
int				handle_heredoc(const char *delimiter, const char *filename);
int				handle_here_doc(const char *delimiter, const char *filename);
// from tmp_heredoc.c
int				handle_here_fork(const char *delimiter, char **temp_file);
int				write_to_temp_file(const char *filename, const char *line);
void			cleanup_temp_file(char *filename);
char			*create_temp_file(void);

// otros utilitarios
void			free_cmds(t_cmd *cmd);
int				ft_isnumeric(const char *str);
void			free_env(char **env);
size_t			ft_strlen(const char *s);
char			*ft_strdup(const char *s);
void			free_cmd_list(t_cmd *cmd);
int				ft_isdigit(int c);
void			free_env(char **env);
void			free_args(char **args);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strcat(char *dest, const char *src);

#endif
