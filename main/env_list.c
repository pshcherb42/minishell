/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:54 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 19:33:14 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*env_new(const char *name, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (name)
		node->name = strdup(name);
	else
		node->name = NULL;
	if (value)
		node->value = strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	env_list_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}

t_env	*env_list_find(t_env *env, const char *name)
{
	while (env)
	{
		if (strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	env_list_set(t_env **env, const char *name, const char *value)
{
	t_env	*node;

	node = env_list_find(*env, name);
	if (node)
	{
		free(node->value);
		if (value)
			node->value = strdup(value);
		else
			node->value = NULL;
		return (0);
	}
	return (env_list_add(env, name, value));
}

int	env_list_add(t_env **env, const char *name, const char *value)
{
	t_env	*new;
	t_env	*last;

	new = env_new(name, value);
	if (!new)
		return (1);
	if (!*env)
	{
		*env = new;
		return (0);
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
	return (0);
}
