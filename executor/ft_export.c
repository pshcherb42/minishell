/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:34:15 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 16:48:18 by akreise          ###   ########.fr       */
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

static	int	process_arg_env(char *arg, t_env **env)
{
	char	*eq;
	int		name_len;
	char	*plus;
	char	*var_name;
	int		result;

	eq = ft_strchr(arg, '=');
	plus = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus)
	{
		name_len = plus - arg;
		return (ft_add_eq(env, arg, name_len));
	}
	if (eq)
	{
		name_len = eq - arg;
		var_name = strndup(arg, name_len);
		if (!var_name)
			return (1);
		result = env_list_set(env, var_name, eq + 1);
		free(var_name);
		return (result);
	}
	else if (!env_list_find(*env, arg))
		return (env_list_add(env, arg, NULL));
	return (0);
}

int	ft_export(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!args[1])
	{
		print_all_env(*env);
		return (0);
	}
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid(args[i]))
		{
			ft_pstr(2, "minishell: export: `");
			ft_pstr(2, args[i]);
			ft_pstr(2, "`: not a valid identifier\n");
			status = 1;
		}
		else if (process_arg_env(args[i], env))
			status = 1;
		i++;
	}
	return (status);
}
