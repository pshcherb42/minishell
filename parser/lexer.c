#include "../minishell.h"

static int  is_quote(char c)
{
    return (c == '\'' || c == '"'); // Verifica si el caracter es comilla
}

char    **split_args(char *input, char **envp, int last_exit_code) // dividir el input, leer las ' y "
{
    char    **args; // array de strings con los argumentos
    char    quote_char = '\0'; // ni idea por ahora
    int     in_quote = 0; // para ver si estas dentro
    int     i = 0, j = 0; // contadores

    args = malloc(sizeof(char *) * 100); // Reservamos espacio para hasta 100 argumentos
    if (!args) // check malloc
        return (NULL);
    
    while (input[i]) // mientras hay input
    {
        // Saltar espacios al inicio de cada argumento (solo si no estamos dentro de comillas)
        while (input[i] == ' ' && !in_quote) // pasar espacios
            i++;
        if (!input[i]) // si no hay inpt salir
            break ;
        int start = i; // contador del principio
        int len = 0; // contdor longitud

        // Aquí recorremos el input hasta encontrar el fin del argumento
        while (input[i])
        {
            if (is_quote(input[i]))
            {
                if (!in_quote) //  Entramos en comillas
                {
                    in_quote = 1; // ahora estamos dentro
                    quote_char = input[i++]; // saltamos la comilla de apertura
                    continue ; // seguimos
                }
                else if (input[i] == quote_char) // Cerramos comillas
                {
                    in_quote = 0;
                    i++; // saltamos la comilla de cierre
                    continue ;
                }
                else
                {
                    // Es una comilla distinta : la dejamos como parte del tokenn
                    i++;
                    continue ;
                }
            }
            if (input[i] == ' ' && !in_quote) // pasams los espacios
                break ;
            i++; // aumentamos i
        } // aumentar i y copiar las quotes en quote char?

        // Copiar el token manualmente sin comillas
        len = i - start; // longitud de la frase
        char *raw_token = malloc(len + 1);
        if (!raw_token) // check duplicado
            return (NULL);
        int k = 0;
        int m = start;
        int inside_quotes = 0;
        char q_char = '\0';

        while (m < i)
        {
            if (is_quote(input[m]))
            {
                if (!inside_quotes)
                {
                    inside_quotes = 1;
                    q_char = input[m++];
                    continue ;
                }
                else if (input[m] == q_char)
                {
                    inside_quotes = 0;
                    m++;
                    continue ;
                }
            }
            raw_token[k++] = input[m++];
        }
        raw_token[k] = '\0';

        // Expand only if not inside single quotes
        char *expanded = raw_token; // guardamos innput en la variable expanded
        if (!(quote_char == '\'')) // comprobamos comillas singles
        {
            expanded = expand_variables(raw_token, envp, last_exit_code); // expandimos variables
            free(raw_token); // liberamos input sin expandir
        }
        args[j++] = expanded; // copiar contenido de la varibale
    }
    args[j] = NULL; // put an end
    return (args); // return
}

// Detecta ' o " y entra/sale de comillas
// Si encuentra un espacio fuera de comillas, cierra el token
// Copia el contenido sin comillas
// Maneja multiples argumentos agrupados, incluso con comillas añadidas correctamente

// Manejar errores de syntaxs
int     validate_quotes(const char *input)
{
    char quote = '\0';
    int i = 0;

    while (input[i])
    {
        if (is_quote(input[i]))
        {
            if (quote == '\0') // Abrimos comillas
                quote = input[i];
            else if (input[i] == quote) // Cerramos comillas
                quote = '\0';
        }
        i++;
    }
    return (quote == '\0'); // Si no hay comillas abiertas, esta bien
}