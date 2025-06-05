/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:34:15 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 20:32:51 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_assignment(t_env **env, char *arg, int name_len)
{
	char	*var_name;
	int		result;

	var_name = strndup(arg, name_len);
	if (!var_name)
		return (1);
	result = env_list_set(env, var_name, arg + name_len + 1);
	free(var_name);
	return (result);
}

static int	process_arg_env(char *arg, t_env **env)
{
	char	*eq;
	int		name_len;
	char	*plus;

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
		return (handle_assignment(env, arg, name_len));
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
