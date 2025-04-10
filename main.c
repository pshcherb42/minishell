#include "minishell.h"

static char **dup_env(char **envp) // Crea una copia completa del array de strings envp
{
    int     count = 0; // contador 
    int     i = 0;
    char    **new_env; // array donde vamos a guardar una copia de los envp

    while (envp[count]) // contamos el tamano de los envp
        count++;
    
    new_env = malloc(sizeof(char *) * (count + 1)); // allocamos un array del tama;o adecuado
    if (!new_env) // check allocation
        return (NULL);
    while (i < count)
    {
        new_env[i] = strdup(envp[i]); // copiamos(cada string individual) el contenido del envp en el new_env
        i++;
    }
    new_env[count] = NULL; // terminamos el array
    return (new_env); // devolvemos el nuevo envp
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

    (void)argc; 
    (void)argv;
    init_shell();

    while (1)
    {
        input = readline("minishell$ "); // que hace esta linea?
        if (!input) // Ctrl-D
        {
            printf("exit\n");
            break ;
        }

        char    *expanded = expand_variables(input, my_env, last_exit_code); // toma input original por ejemplo echo $VAR y lo transforma en echo hello
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
