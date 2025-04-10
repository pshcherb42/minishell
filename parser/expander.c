#include "../minishell.h"
#include <ctype.h>

char    *get_env_value(const char *var, char **envp)
{
    int     i; // contador
    int     len; // cotador length

    if (!var || !var[0]) // check leak
        return (strdup("")); // porque devolvemos un duplicado?

    len = strlen(var); // longitud de var
    for (i = 0; envp[i]; i++) // pasamos por todas las palabras de los envp
    {
        if (!strncmp(envp[i], var, len) && envp[i][len] == '=') // si el var coincide con el input y despues tiene un igual
            return (strdup(envp[i] + len + 1)); // var = a lo que va despues del '=' 
    }
    return (strdup("")); // si no se encuentra, se reemplaza por cadena vacia
}

char    *expand_variables(const char *input, char **envp, int last_exit_code)
{
    char    *result; // expanded 
    int     i = 0, j = 0; // contadores
    //char    *final_result;

    result = malloc(4096); // buffer simple
    if (!result) // check malloc
        return (NULL);

    while (input[i]) // Leemos el ./minishell: input
    {
        if (input[i] == '$') // es un signo de dollar?
        {
            i++; // avanzamos una posicion
            if (input[i] == '?') // caso especial $?
            {
                j += snprintf(result + j, 4096 - j, "%d", last_exit_code); // Copiamos el contenido de last exit code en result a partir de la posicion j
                i++; // avanzamos una posicion
            }
            else
            {
                char var[256]; // un array de 256 chracteres
                int k = 0; // contador

                while (input[i] && (isalnum(input[i]) || input[i] == '_')) // leer el input y copiar en el array mientras son numeros o '_'
                    var[k++] = input[i++]; // var ahora es igual a la primera parte del input, la que contiene el nombre de la variable
                var[k] = '\0'; // terminar el array
                const char *val = get_env_value(var, envp); // pasamos las my_env y la primer parte del input a la fucion que busca dentro del los my_envp la palabra que coincida y la guarda en val
                j += snprintf(result + j, 4096 - j, "%s", val); // copia el resultado del val(la palabra clave), en el resultado
                free((char *)val); // liberar val
            }
        }
        else // en otro caso
            result[j++] = input[i++]; // copiamos input en result
    }
    result[j] = '\0'; // Terminamos el string
    //final_result = strdup(result); // duplicate result
    //free(result); // free temporary buffer
    return (result); // devolvemos o el exit code o lo que va despues del nombre de la variable.
}
