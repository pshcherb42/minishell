/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 18:53:37 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	replace_new_var(char ***envp, char *arg, int name_len)
{
	int		i;
	char	*new;
	char	*eq_in_arg;
	char	*eq_in_existing;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], arg, name_len) &&
			((*envp)[i][name_len] == '=' || (*envp)[i][name_len] == '\0'))
		{
			eq_in_arg = ft_strchr(arg, '=');
			eq_in_existing = ft_strchr((*envp)[i], '=');
			if (!eq_in_arg && eq_in_existing)
				return (1);
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

static int	copy_env_var(char ***envp, char **new_env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup((*envp)[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (-1);
		}
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
	i = copy_env_var(envp, new_env, count);
	if (i == -1)
		return (1);
	new_env[i] = ft_strdup(arg);
	if (!new_env[i])
	{
		free_args(new_env);
		return (1);
	}
	new_env[i + 1] = NULL;
	free_args(*envp);
	*envp = new_env;
	return (0);
}

void	print_all_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("declare -x %s=\"%s\"\n", env->name, env->value);
		else
			ft_printf("declare -x %s\n", env->name);
		env = env->next;
	}
}
