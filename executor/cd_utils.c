/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/21 15:13:23 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Создает строку вида VAR=VALUE
static char	*join_env_entry(const char *var_name, const char *value)
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

void	replace_env(const char *var_name, const char *value, char **envp)
{
	int		i;
	size_t	len;
	char	*new_entry;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
		{
			free(envp[i]);
			new_entry = join_env_entry(var_name, value);
			if (!new_entry)
				return ;
			envp[i] = new_entry;
			return ;
		}
		i++;
	}
	new_entry = join_env_entry(var_name, value);
	if (!new_entry)
		return ;
	envp[i] = new_entry;
	envp[i + 1] = NULL;
}

// Получает путь для "cd -" из переменной окружения OLDPWD
static	char	*get_oldpwd(void)
{
	char	*oldpwd;

	oldpwd = getenv("OLDPWD"); // Получаем OLDPWD из envp
	if (!oldpwd)
		return (NULL);
	ft_pstr(1, oldpwd);  // Печатаем старую директорию
	ft_pstr(1, "\n");
	return (ft_strdup(oldpwd));  // Возвращаем старую директорию
}

static	char	*home_tilde(char *arg)
{
	char	*home;
	char	*rest;
	char	*full_path;
	size_t	home_len;
	size_t	rest_len;

	home = getenv("HOME");
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

// Получает целевой путь для смены директории (для cd - или из аргументов)
char	*get_target_path(char **args)
{
	char	*home;

	if (args[1] && ft_strcmp(args[1], "-") == 0)
		return (get_oldpwd());
	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
			return (NULL);
		return (ft_strdup(home));
	}
	if (args[1][0] == '~')
		return (home_tilde(args[1]));
	return (ft_strdup(args[1]));
}
