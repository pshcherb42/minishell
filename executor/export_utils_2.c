/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/05/09 17:58:15 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*prep_joined(const char *arg, const char *var_name)
{
	int		name_len;
	char	*joined;

	name_len = ft_strlen(var_name);
	joined = ft_strdup(arg + name_len + 2); // пропускаем +=
	return (joined);
}

int	add_if_needed(char ***envp, char *arg)
{
	if (add_env_var(envp, arg))
		return (1);
	return (0);
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

// Проверяет, существует ли переменная с именем `var_name` в envp
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
