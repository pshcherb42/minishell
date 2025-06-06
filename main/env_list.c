/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:54 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 18:53:49 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env *env_new(const char *name, const char *value)
{
	t_env *node = malloc(sizeof(t_env));
	if (!node) return NULL;
	node->name = name ? strdup(name) : NULL;
	node->value = value ? strdup(value) : NULL;
	node->next = NULL;
	return node;
}

void env_list_free(t_env *env)
{
	t_env *tmp;
	while (env) {
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}

t_env *env_list_find(t_env *env, const char *name)
{
	while (env) {
		if (strcmp(env->name, name) == 0)
			return env;
		env = env->next;
	}
	return NULL;
}

int env_list_set(t_env **env, const char *name, const char *value)
{
	t_env *node = env_list_find(*env, name);
	if (node) {
		free(node->value);
		node->value = value ? strdup(value) : NULL;
		return 0;
	}
	return env_list_add(env, name, value);
}

int env_list_add(t_env **env, const char *name, const char *value)
{
	t_env *new = env_new(name, value);
	t_env *last;
	if (!new) return 1;
	if (!*env) {
		*env = new;
		return 0;
	}
	last = *env;
	while (last->next) last = last->next;
	last->next = new;
	return 0;
}

int env_list_unset(t_env **env, const char *name) {
	t_env *cur = *env, *prev = NULL;
	while (cur) {
		if (strcmp(cur->name, name) == 0) {
			if (prev) prev->next = cur->next;
			else *env = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return 0;
		}
		prev = cur;
		cur = cur->next;
	}
	return 1;
}

// Helper to split "NAME=VALUE" into name and value
static void split_env_entry(const char *entry, char **name, char **value)
{
	const char *eq = strchr(entry, '=');
	if (eq) {
		*name = strndup(entry, eq - entry);
		*value = strdup(eq + 1);
	} else {
		*name = strdup(entry);
		*value = NULL;
	}
}

t_env *env_list_from_array(char **envp)
{
	t_env *head = NULL;
	int i = 0;

	while (envp && envp[i])
	{
		char *name = NULL, *value = NULL;
		split_env_entry(envp[i], &name, &value);
		env_list_add(&head, name, value);
		free(name);
		free(value);
		i++;
	}
	init_minimal_env_list(&head);
	return head;
}

void init_minimal_env_list(t_env **env)
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

char **env_list_to_array(t_env *env)
{
	int count = 0;
	t_env *tmp = env;
	char **arr;
	int i = 0;
	while (tmp) { count++; tmp = tmp->next; }
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr) return NULL;
	tmp = env;
	while (tmp) {
		size_t len = strlen(tmp->name) + 1 + (tmp->value ? strlen(tmp->value) : 0);
		arr[i] = malloc(len + 1);
		if (tmp->value)
			snprintf(arr[i], len + 1, "%s=%s", tmp->name, tmp->value);
		else
			snprintf(arr[i], len + 1, "%s", tmp->name);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

