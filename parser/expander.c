#include "../minishell.h"
#include <ctype.h>

char    *get_env_value(const char *var, char **envp)
{
    int     i;
    int     len;

    len = strlen(var);
    for (i = 0; envp[i]; i++)
    {
        if (!strncmp(envp[i], var, len) && envp[i][len] == '=') // que hace esta linea?
            return (strdup(envp[i] + len + 1)); //libft
    }
    return (strdup("")); // si no se encuentra, se reemplaza por cadena vacia
}

char    *expand_variables(const char *input, char **envp, int last_exit_code)
{
    char    *result = malloc(4096); // buffer simple
    int     i = 0, j = 0;

    while (input[i])
    {
        if (input[i] == '$')
        {
            i++;
            if (input[i] == '?') // caso especial $?
            {
                j += snprintf(result + j, 4096 - j, "%d", last_exit_code); // para $?
                i++;
            }
            else
            {
                char var[256];
                int k = 0;

                while (input[i] && (isalnum(input[i]) || input[i] == '_')) // libft, porque _? y porque numero?
                    var[k++] = input[i++];
                var[k] = '\0'; // no se puede pasar input directamente?
                const char *val = get_env_value(var, envp); // variable
                j += snprintf(result + j, 4096 - j, "%s", val); // para $VAR
                free((char *)val); // si fue duplicado
            }
        }
        else
            result[j++] = input[i++]; // copiamos input en result
    }
    result[j] = '\0';
    return (strdup(result)); // libft
}
