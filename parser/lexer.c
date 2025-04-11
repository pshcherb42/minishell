#include "../minishell.h"

static int  is_quote(char c)
{
    return (c == '\'' || c == '"');
}

char    **split_args(char *input, char **envp, int last_exit_code) // dividir el input, leer las ' y "
{
    char    **args;
    char    quote_char = '\0';
    int     in_quote = 0;
    int     i = 0, j = 0;

    args = malloc(sizeof(char *) * 100); // hasta 100 argumentos; esta permitido? allocamos espacio para 100 argumentos
    if (!args) // check malloc
        return (NULL);
    
    while (input[i]) // mientras hay palabras
    {
        // Saltar espacios al inicio de un nuevo token (fuera comillas
        while (input[i] == ' ' && !in_quote)
            i++;
        if (!input[i])
            break ;
        int start = i;
        int len = 0;
        while (input[i])
        {
            if (is_quote(input[i]))
            {
                if (!in_quote)
                {
                    in_quote = 1;
                    quote_char = input[i++];
                    continue ;
                }
                else if (input[i] == quote_char)
                {
                    in_quote = 0;
                    quote_char = '\0';
                    i++;
                    continue ; 
                }
            }
            
            if (input[i] == ' ' && !in_quote)
                break ;
            else
                i++;
        }

        // Copiar el token manualmente
        len = i - start;
        char *raw_token = strndup(input + start, len);
        if (!raw_token)
            return (NULL);

        // Expand only if not inside single quotes
        char *expanded = raw_token;
        if (!(quote_char == '\''))
        {
            expanded = expand_variables(raw_token, envp, last_exit_code);
            free(raw_token);
        }
        args[j++] = expanded;
    }
    args[j] = NULL; // put an end
    return (args); // return
}

// Detecta ' o " y entra/sale de comillas
// Si encuentra un espacio fuera de comillas, cierra el token
// Copia el contenido sin comillas
// Maneja multiples argumentos agrupados, incluso con comillas añadidas correctamente
