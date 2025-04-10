#include "../minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (
        !strcmp(cmd, "cd") ||
        !strcmp(cmd, "exit") ||
        !strcmp(cmd, "pwd") ||
        !strcmp(cmd, "echo") ||
        !strcmp(cmd, "env") ||
        !strcmp(cmd, "unset") ||
        !strcmp(cmd, "export")
    ); // libft
}

int exec_builtin(t_cmd *cmd, char **envp)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (1);
    if (!strcmp(cmd->args[0], "exit")) // libft
    {
        printf("exit\n");
        if (cmd->args[1])
        {
            for (int i = 0; cmd->args[1][i]; i++)
            {
                if (!ft_isdigit(cmd->args[1][i]))
                {
                    fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmd->args[1]); // cambiar a algo permitido
                    exit(255);
                }
            }
            if (cmd->args[2])
            {
                fprintf(stderr, "minishell: exit: too many arguments\n");
                return (1);
            }
            exit(atoi(cmd->args[1]) % 256); // que hace esto? porque 256?
        }
        exit (0);
    }
    else if (!strcmp(cmd->args[0], "cd")) // libft
        return (ft_cd(cmd->args, envp)); // <- pasamos args y envp, luego guardar envp como cmd->env
    else if (!strcmp(cmd->args[0], "pwd")) //libft
        return (ft_pwd());
    else if (!strcmp(cmd->args[0], "echo")) // libft
        return (ft_echo(cmd->args));
    else if (!strcmp(cmd->args[0], "env"))
        return (ft_env(envp));
    else if (!strcmp(cmd->args[0], "unset"))
        return (ft_unset(cmd->args, &envp));
    else if (!strcmp(cmd->args[0], "export"))
        return (ft_export(cmd->args, &envp));
    return (1);
}

int ft_cd(char **args, char **envp)
{
    char    *target;
    int     ret;
    (void)envp;

    if (!args[1]) // solo "cd"
    {
        target = getenv("HOME");
        if (!target)
        {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            return (1);
        }
    }
    else
        target = args[1];

    ret = chdir(target);
    if (ret != 0)
    {
        perror("minishell: cd");
        return (1);
    }
    // Bonus: actualizar PWD en entorno, luego lo haremos
    return (0);
}

int ft_pwd(void)
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) // que hace getcwd?
    {
        perror("minishell: pwd");
        return (1);
    }
    printf("%s\n", cwd);
    return (0);
}

int ft_echo(char **args)
{
    int i = 1;
    int newline = 1;

    // Manejo de multiples -n seguidos
    if (args[i] && strncmp(args[i], "-n", 2) == 0)
    {
        while (args[i] && strspn(args[i], "-n") == strlen(args[i])) // verifica que el argumento esté formado solo por - y n, cubre casos como -n, -nnn etc.
        {
            newline = 0;
            i++;
        }
    }

    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" "); // entre args, pero no despues del ultimo
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}

int ft_env(char **envp)
{
    for (int i = 0; envp[i]; i++)
    {
        // Solo imprimimos las que tienen '='
        if (strchr(envp[i], '=')) // libft
            printf("%s\n", envp[i]);
    }
    return (0);
}

int ft_unset(char **args, char ***envp)
{
    int i = 1;
    int j, k;

    if (!args[1])
        return (0);
    
    while (args[i])
    {
        j = 0;
        while ((*envp)[j])
        {
            if (!strncmp((*envp)[j], args[i], strlen(args[i])) &&
                (*envp)[j][strlen(args[i])] == '=')
            {
                free((*envp)[j]);
                for (k = j; (*envp)[k]; k++)
                    (*envp)[k] = (*envp)[k + 1];
                continue ; // revisa la nueva linea en j
            }
            j++;
        }
        i++;
    }
    return (0);
}

int ft_export(char **args, char ***envp)
{
    int i = 1;

    // Si no hay argumentos, imprimir todas las vars
    if (!args[1])
    {
        for (int j = 0; (*envp)[j]; j++)
            printf("declare -x %s\n", (*envp)[j]);
        return (0);
    }

    while (args[i])
    {
        char    *eq = strchr(args[i], '=');
        if (eq)
        {
            int     name_len = eq - args[i]; // Correct variable name length
            int found = 0;

            // Ver si la varibale ya existe -> reemplazarla
            for (int j = 0; (*envp)[j]; j++)
            {
                // Compare only the variable name part
                if (!strncmp((*envp)[j], args[i], name_len) &&
                (*envp)[j][name_len] == '=')
                {
                    char *new = strdup(args[i]);
                    if (!new)
                        return (1);
                    free((*envp)[j]);
                    (*envp)[j] = new;
                    found = 1;
                    break ;
                }
            }

            // Si no existe -> añdirla
            if (!found)
            {
                int count = 0;
                while ((*envp)[count])
                    count++;

                char **new_env = malloc(sizeof(char *) * (count + 2));
                if (!new_env)
                    return (1);
                
                // Copy all existing environment variables
                for (int k = 0; k < count; k++)
                    new_env[k] = (*envp)[k];
                
                // Add new variable
                new_env[count] = strdup(args[i]);
                if (!new_env[count])
                {
                    free(new_env);
                    return (1);
                }
                new_env[count + 1] = NULL;

                // Guardamos el antiguo array para liberar luego
                //free_env(*envp);
                *envp = new_env;
                //free_env(*envp);
                // Liberar solo el array viejo, no las cadenas que ya estan copiadas
            }
        }
        // Si no hay '=', se ignora (vérsion básica; se puede ampliar luego)
        i++;
    }
    return (0);
}