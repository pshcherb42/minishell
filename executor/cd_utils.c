/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/31 19:46:52 by akreise          ###   ########.fr       */
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

static int	print_cd_env_error(const char *var_name)
{
	ft_pstr(2, "minishell: cd: ");
	ft_pstr(2, var_name);
	ft_pstr(2, " not set\n");
	return (1);
}

static char	*handle_special_args(char **args, char **envp)
{
	char	*var;

	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		var = find_env_var_local(envp, "OLDPWD");
		if (!var)
		{
			print_cd_env_error("OLDPWD");
			return (NULL);
		}
		return (ft_strdup(var));
	}
	if (args[1] && args[1][0] == '\0')
		return (ft_strdup("."));
	if (!args[1])
	{
		var = find_env_var_local(envp, "HOME");
		if (!var)
		{
			print_cd_env_error("HOME");
			return (NULL);
		}
		return (ft_strdup(var));
	}
	return (NULL);
}

char	*get_target_path(char **args, char **envp)
{
	char	*result;

	result = handle_special_args(args, envp);
	if (result)
		return (result);
	if (args[1][0] == '~')
		return (home_tilde(args[1], envp));
	return (ft_strdup(args[1]));
}
