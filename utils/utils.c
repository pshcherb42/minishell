#include "../minishell.h"

void    free_cmds(t_cmd *cmd)
{
    int i = 0;

    if (!cmd)
        return ;
    if (cmd->args)
    {
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
    }
    free(cmd);
}

int     ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

void    free_env(char **env)
{
    int i = 0;
    while (env[i])
        free(env[i++]);
    free(env);
}