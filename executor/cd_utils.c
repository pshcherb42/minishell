/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 20:25:44 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_env_entry(const char *var_name, const char *value)
{
	char	*entry;
	size_t	var_len;
	size_t	val_len;

	var_len = ft_strlen(var_name);
	val_len = ft_strlen(value);
	entry = malloc(var_len + val_len + 2);
	if (!entry)
		return (NULL);
	ft_memcpy(entry, var_name, var_len);
	entry[var_len] = '=';
	ft_memcpy(entry + var_len + 1, value, val_len);
	entry[var_len + val_len + 1] = '\0';
	return (entry);
}

static char	*find_home_value(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (strcmp(cur->name, "HOME") == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

static char	*home_tilde_env(char *arg, t_env *env)
{
	char	*home;
	char	*rest;
	char	*full_path;
	size_t	home_len;
	size_t	rest_len;

	home = find_home_value(env);
	if (!home)
		return (NULL);
	rest = arg + 1;
	home_len = ft_strlen(home);
	rest_len = ft_strlen(rest);
	full_path = malloc(home_len + rest_len + 1);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, home, home_len);
	ft_memcpy(full_path + home_len, rest, rest_len);
	full_path[home_len + rest_len] = '\0';
	return (full_path);
}

char	*get_target_path_env(char **args, t_env *env)
{
	char	*result;
	int		error_flag;

	error_flag = 0;
	result = handle_special_args_env(args, env, &error_flag);
	if (result)
		return (result);
	if (error_flag)
		return (NULL);
	if (args[1] && args[1][0] == '~')
		return (home_tilde_env(args[1], env));
	if (args[1])
		return (ft_strdup(args[1]));
	return (NULL);
}
