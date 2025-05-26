/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/26 20:13:45 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Создает строку вида VAR=VALUE
char	*join_env_entry(const char *var_name, const char *value)
{
	char	*entry;
	size_t	var_len;
	size_t	val_len;

	var_len = ft_strlen(var_name);
	val_len = ft_strlen(value);
	entry = malloc(var_len + val_len + 2); // +1 for '=' and +1 for '\0'
	if (!entry)
		return (NULL);
	ft_memcpy(entry, var_name, var_len);
	entry[var_len] = '=';
	ft_memcpy(entry + var_len + 1, value, val_len);
	entry[var_len + val_len + 1] = '\0';
	return (entry);
}

// Получает путь для "cd -" из локального envp
static char	*get_oldpwd(char **envp)
{
	char	*oldpwd;

	oldpwd = find_env_var_local(envp, "OLDPWD");
	if (!oldpwd)
		return (NULL);
	return (ft_strdup(oldpwd));
}

static char	*home_tilde(char *arg, char **envp)
{
	char	*home;
	char	*rest;
	char	*full_path;
	size_t	home_len;
	size_t	rest_len;

	home = find_env_var_local(envp, "HOME");
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

// Получает целевой путь для смены директории
char	*get_target_path(char **args, char **envp)
{
	char	*home;

	if (args[1] && ft_strcmp(args[1], "-") == 0)
		return (get_oldpwd(envp));
	if (!args[1])
	{
		home = find_env_var_local(envp, "HOME");
		if (!home)
			return (NULL);
		return (ft_strdup(home));
	}
	if (args[1][0] == '~')
		return (home_tilde(args[1], envp));
	return (ft_strdup(args[1]));
}
