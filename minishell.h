/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:07 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/26 18:03:51 by akreise          ###   ########.fr       */
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

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

// funciones de main
void	init_shell(void);

// funciones parser
t_cmd	*parse_single_command(char *input, char **envp, int last_exit_code);
t_cmd	*parse_input(char *input, char **envp, int last_exit_code);
char	**split_args(char *input, char **envp, int last_exit_code);
char	*expand_variables(const char *input, char **envp, int last_exit_code);
char	*get_env_value(const char *var, char **envp);
int		validate_quotes(const char *input);

// funciones ejecucción
int		open_redirs(t_cmd *cmd);
int		execute_cmds(t_cmd *cmd, char ***envp);
char	*join_path(const char *dir, const char *cmd);
char	**split_path(const char *path);
void	free_split(char **arr);
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

// otros utilitarios
void	free_cmds(t_cmd *cmd);
int		ft_isnumeric(const char *str);
void	free_env(char **env);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *nptr);
size_t	ft_strlen(const char *s);
size_t	ft_strspn(const char *s, const char *accept);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
void	ft_pstr(int fd, const char *str);
#endif
