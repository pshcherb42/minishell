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

typedef struct s_cmd
{
    char **args;
    char *infinle;
    char *outfile;
    int append;
    int heredoc;
    struct s_cmd *next;
}   t_cmd;

// funciones de main
void    init_shell(void);

// funciones parser
t_cmd   *parse_input(char *input);
char    **split_args(char *input);
char	*expand_variables(const char *input, char **envp, int last_exit_code);
char    *get_env_value(const char *var, char **envp);

// funciones ejecucción
int     execute_cmds(t_cmd *cmd, char **envp);
char	*get_cmd_path(char *cmd, char **envp);
int     ft_cd(char **args, char **envp);
int     ft_pwd(void);
int     ft_echo(char **args);
int     ft_env(char **envp);
int     ft_unset(char **args, char ***envp);
int     ft_export(char **args, char ***envp);

// funciones de built-ins
int     is_builtin(char *cmd);
int     exec_builtin(t_cmd *cmd, char **envp);

// funciones de señales
void    setup_signals(void);

// otros utilitarios
void    free_cmds(t_cmd *cmd);
int     ft_isdigit(int c);
void    free_env(char **env);

#endif