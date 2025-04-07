#include "minishell.h"

static char **dup_env(char **envp)
{
    int     count = 0;
    char    **new_env;

    while (envp[count])
        count++;
    
    new_env = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++) // cambiar
        new_env[i] = strdup(envp[i]); // libft
    new_env[count] = NULL;
    return (new_env);
}

void    init_shell(void)
{
    setup_signals();
}

int main(int argc, char **argv, char **envp)
{
    char    *input;
    t_cmd   *cmds; // no entiendo porque lo tenemos que crear de nuevo si tenemos la esructura definida en .h, porque .h = plano del edificia, .c = construccion real del edificio
    int     last_exit_code = 0;
    char    **my_env = dup_env(envp);
    
    (void)argc; // porque void? no es una declaracion? para silenciar warnings. unused parameter
    (void)argv; // porque sin punteros? para silenciar los warnings unused variable
    init_shell();

    while (1)
    {
        input = readline("minishell$ "); // que hace esta linea?
        if (!input) // Ctrl-D
        {
            printf("exit\n");
            break ;
        }

        char    *expanded = expand_variables(input, my_env, last_exit_code);
        if (*input)
            add_history(input);
        cmds = parse_input(expanded);
        if (cmds)
            last_exit_code = execute_cmds(cmds, my_env);
        free_cmds(cmds);
        free(expanded);
        free(input);
    }
    free_env(my_env); // crea una funcion para liberar env
}
