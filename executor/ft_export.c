/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:34:15 by akreise           #+#    #+#             */
/*   Updated: 2025/04/30 19:20:59 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_all(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
}

int	replace_new_var(char ***envp, char *arg, int name_len)
{
	int		i;
	char	*new;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], arg, name_len) &&
			(*envp)[i][name_len] == '=')
		{
			new = ft_strdup(arg);
			if (!new)
				return (1);
			free((*envp)[i]);
			(*envp)[i] = new;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	copy_env_var(char ***envp, char **new_env)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	return (i);
}

int	add_env_var(char ***envp, char *arg)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while ((*envp)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = copy_env_var(envp, new_env);
	new_env[i] = ft_strdup(arg);
	if (!new_env[i])
	{
		free(new_env);
		return (1);
	}
	new_env[i + 1] = NULL;
	*envp = new_env;
	return (0);
}

int	ft_export(char **args, char ***envp) // adds/modifies environment variables
{
	int		i;
	char	*eq;
	int		name_len;

	i = 1;
	if (!args[1])
	{
		print_all(*envp);
		return (0);
	}
	while (args[i])
	{
		eq = strchr(args[i], '=');
		if (eq)
		{
			name_len = eq - args[i];
			if (!replace_new_var(envp, args[i], name_len))
			{
				if (!add_env_var(envp, args[i]))
					return (1);
			}
		}
		i++;
	}
	return (0);
}
