/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/05/26 20:38:49 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_env_var_local(char **envp, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static int	update_existing_env(char **envp, const char *var_name,
		const char *value, size_t len)
{
	int		i;
	char	*new_entry;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
		{
			free(envp[i]);
			new_entry = join_env_entry(var_name, value);
			if (!new_entry)
				return (0);
			envp[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

static void	add_new_env(char **envp, const char *var_name, const char *value)
{
	int		i;
	char	*new_entry;

	i = 0;
	while (envp[i])
		i++;
	new_entry = join_env_entry(var_name, value);
	if (!new_entry)
		return ;
	envp[i] = new_entry;
	envp[i + 1] = NULL;
}

void	replace_env(const char *var_name, const char *value, char **envp)
{
	size_t	len;

	len = ft_strlen(var_name);
	if (update_existing_env(envp, var_name, value, len))
		return ;
	add_new_env(envp, var_name, value);
}

void	update_env_vars(char *oldpwd, char **envp)
{
	char	*newpwd;

	newpwd = get_current_dir();
	if (!newpwd)
		return ;
	replace_env("OLDPWD", oldpwd, envp);
	replace_env("PWD", newpwd, envp);
	free(newpwd);
}
