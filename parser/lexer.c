#include "../minishell.h"

char    **split_args(char *input)
{
    char    **args;
    char    *token;
    int     i = 0;

    args = malloc(sizeof(char *) * 100); // hasta 100 argumentos; esta permitido? allocamos espacio para 100 argumentos
    if (!args) // check malloc
        return (NULL);
    
    token = strtok(input, " "); // divide el string por espacios
    while (token) // mientras hay palabras
    {
        args[i++] = strdup(token); // copiamos argumentos en la varibale token
        token = strtok(NULL, " "); // liberamos la variable?
    }
    args[i] = NULL; // put an end
    return (args); // return
}