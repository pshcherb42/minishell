#include "../minishell.h"

static char     *join_path(const char *dir, const char *cmd)
{
    char    *full;
    int     len;

    len = strlen(dir) + strlen(cmd) + 2; // libft
    full = malloc(len);
    if (!full)
        return (NULL);
    snprintf(full, len, "%s/%s", dir, cmd); // wtf is this? cambiar por algo permitido
    return (full);
}

static char     **split_path(const char *path)
{
    char    **result;
    char    *copy = strdup(path); // libft
    char    *token;
    int     i = 0;

    result = malloc(sizeof(char *) * 100); // permite hasta 100 directorios PATH ; esto esta permitido? averiguar
    if (!result)
        return (NULL);
    
    token = strtok(copy, ":"); // otra mierda rara, cambiar por algo permitido
    while (token)
    {
        result[i++] = strdup(token); // libft
        token = strtok(NULL, ":"); // oktorok
    }
    result[i] = NULL;
    free(copy); // luego dara problemas?
    return (result);
}

static void     free_split(char **arr) // Pirate said: ARR!
{
    int i = 0;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}

char    *get_cmd_path(char *cmd, char **envp)
{
    char    *path_env = NULL;
    char    **paths = NULL;
    char    *full_path = NULL;
    int     i = 0;

    if (strchr(cmd, '/')) // libft
        return (strdup(cmd)); // libft

    while (envp[i])
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break ;
        }
        i++;
    }
    if (!path_env)
        return (NULL);

    paths = split_path(path_env);
    for (i = 0; paths[i]; i++) // norminette
    {
        full_path = join_path(paths[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            free_split(paths);
            return (full_path);
        }
        free(full_path);
    }
    free_split(paths);
    return (NULL);
}