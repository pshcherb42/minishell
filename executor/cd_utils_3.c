/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/06/03 16:18:14 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_cd_env_error(const char *var_name)
{
	ft_pstr(2, "minishell: cd: ");
	ft_pstr(2, var_name);
	ft_pstr(2, " not set\n");
	return (1);
}

static char	*handle_dash_arg(char **envp, int *error_flag)
{
	char	*var;

	var = find_env_var_local(envp, "OLDPWD");
	if (!var)
	{
		print_cd_env_error("OLDPWD");
		*error_flag = 1;
		return (NULL);
	}
	return (ft_strdup(var));
}

static char	*handle_no_arg(char **envp, int *error_flag)
{
	char	*var;

	var = find_env_var_local(envp, "HOME");
	if (!var)
	{
		print_cd_env_error("HOME");
		*error_flag = 1;
		return (NULL);
	}
	return (ft_strdup(var));
}

char	*handle_special_args(char **args, char **envp, int *error_flag)
{
	if (args[1] && ft_strcmp(args[1], "-") == 0)
		return (handle_dash_arg(envp, error_flag));
	if (args[1] && args[1][0] == '\0')
		return (ft_strdup("."));
	if (!args[1])
		return (handle_no_arg(envp, error_flag));
	return (NULL);
}
