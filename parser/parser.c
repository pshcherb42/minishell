#include "../minishell.h"

t_cmd   *parse_input(char *input)
{
    t_cmd *cmd = malloc(sizeof(t_cmd)); // creamos malloc para la lista
    if (!cmd) // para que crear una nueva estructura si ya tenemos una definida?
        return (NULL);
    cmd->args = split_args(input);
    cmd->infinle = NULL;
    cmd->outfile = NULL;
    cmd->append = 0; // a que corresponde append?
    cmd->heredoc = 0;
    cmd->next = NULL;
    return (cmd);
}