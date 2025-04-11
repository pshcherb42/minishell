#include "../minishell.h"

t_cmd   *parse_input(char *input, char **envp, int last_exit_code)
{
    t_cmd *cmd = malloc(sizeof(t_cmd)); // creamos malloc para la lista
    if (!cmd) // check malloc
        return (NULL);
    cmd->args = split_args(input, envp, last_exit_code);
    cmd->infinle = NULL;
    cmd->outfile = NULL;
    cmd->append = 0; // a que corresponde append?
    cmd->heredoc = 0;
    cmd->next = NULL;
    return (cmd);
}