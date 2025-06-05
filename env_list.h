#ifndef ENV_LIST_H
#define ENV_LIST_H

#include <stdlib.h>
# include "minishell.h"

typedef struct s_env {
    char *name;
    char *value;
    struct s_env *next;
} t_env;

t_env   *env_list_from_array(char **envp);
char    **env_list_to_array(t_env *env);
void    env_list_free(t_env *env);
t_env   *env_list_find(t_env *env, const char *name);
int     env_list_set(t_env **env, const char *name, const char *value);
int     env_list_unset(t_env **env, const char *name);
int     env_list_add(t_env **env, const char *name, const char *value);

#endif
