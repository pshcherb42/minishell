#include "../minishell.h"

int execute_cmds(t_cmd *cmd, char ***envp)
{
    pid_t   pid; // señal procesos
    int     status; // variable
    char    *path; // string

    if (!cmd || !cmd->args || !cmd->args[0]) // error check
        return (1);
    
    if (is_builtin(cmd->args[0])) // esto ejecuta builtin
        return (exec_builtin(cmd, envp)); // porque solo hacemos fork para comandos? Y que coño es un builtin? ñ

    path = get_cmd_path(cmd->args[0], *envp);
    if (!path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
        return (127);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (1);
    }
    else if (pid == 0)
    {
        // hijo
        execve(path, cmd->args, *envp);
        perror("execve"); // solo si falla
        exit(EXIT_FAILURE);
    }
    else
        waitpid(pid, &status, 0); // porque el padre espera?
    free(path);
    return (WEXITSTATUS(status)); // retorna el status del hijo, es util para $? despues
}