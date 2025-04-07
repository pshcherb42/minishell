#include "../minishell.h"

char    **split_args(char *input)
{
    char    **args;
    char    *token;
    int     i = 0;

    args = malloc(sizeof(char *) * 100); // hasta 100 argumentos; esta permitido?
    if (!args)
        return (NULL);
    
    token = strtok(input, " "); // divide el string por espacios
    while (token)
    {
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return (args);
}