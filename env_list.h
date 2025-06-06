/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:07 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 19:24:50 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_LIST_H
# define ENV_LIST_H

# include "minishell.h"

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*env_list_from_array(char **envp);
char	**env_list_to_array(t_env *env);
void	env_list_free(t_env *env);
void	init_minimal_env_list(t_env **env);
t_env	*env_list_find(t_env *env, const char *name);
int		env_list_set(t_env **env, const char *name, const char *value);
int		env_list_unset(t_env **env, const char *name);
int		env_list_add(t_env **env, const char *name, const char *value);

#endif
