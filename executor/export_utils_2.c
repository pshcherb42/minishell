/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/06/03 18:24:09 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*prep_joined(const char *arg, const char *var_name)
{
	int		name_len;
	char	*joined;

	name_len = ft_strlen(var_name);
	joined = ft_strdup(arg + name_len + 2);
	return (joined);
}

int	add_if_needed_env(t_env **env, char *arg)
{
	char *eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	int name_len = eq - arg;
	char *name = strndup(arg, name_len);
	char *value = strdup(eq + 1);
	int res = env_list_set(env, name, value);
	free(name);
	free(value);
	return res;
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;

	if (!s1 || !s2)
		return (NULL);
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

int	var_exists(char **envp, const char *var_name)
{
	int		i;
	int		len;

	len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var_name, len)
			&& (envp[i][len] == '=' || envp[i][len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

int	is_valid(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (1);
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
