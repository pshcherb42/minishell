/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:44 by akreise           #+#    #+#             */
/*   Updated: 2025/06/06 19:35:35 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_list_unset(t_env **env, const char *name)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (1);
}

static void	split_env_entry(const char *entry, char **name, char **value)
{
	const char	*eq = ft_strchr(entry, '=');

	if (eq)
	{
		*name = ft_strndup(entry, eq - entry);
		*value = ft_strdup(eq + 1);
	}
	else
	{
		*name = ft_strdup(entry);
		*value = NULL;
	}
}

t_env	*env_list_from_array(char **envp)
{
	t_env	*head;
	int		i;
	char	*name;
	char	*value;

	head = NULL;
	i = 0;
	while (envp && envp[i])
	{
		name = NULL;
		value = NULL;
		split_env_entry(envp[i], &name, &value);
		env_list_add(&head, name, value);
		free(name);
		free(value);
		i++;
	}
	init_minimal_env_list(&head);
	return (head);
}

void	init_minimal_env_list(t_env **env)
{
	char	*cwd;
	char	*current_shlvl;

	if (!env_list_find(*env, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			env_list_add(env, "PWD", cwd);
			free(cwd);
		}
		else
			env_list_add(env, "PWD", "/");
	}
	current_shlvl = find_env_var(*env, "SHLVL");
	if (!current_shlvl)
		env_list_add(env, "SHLVL", "0");
	if (!env_list_find(*env, "_"))
		env_list_add(env, "_", "./minishell");
}
