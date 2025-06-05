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

/*static int	print_cd_env_error(const char *var_name)
{
	ft_pstr(2, "minishell: cd: ");
	ft_pstr(2, var_name);
	ft_pstr(2, " not set\n");
	return (1);
}*/

static char *find_env_value_env(t_env *env, const char *name) {
	t_env *cur = env;
	while (cur) {
		if (strcmp(cur->name, name) == 0)
			return cur->value;
		cur = cur->next;
	}
	return NULL;
}

static char *handle_dash_arg_env(t_env *env, int *error_flag) {
	char *var = find_env_value_env(env, "OLDPWD");
	if (!var) {
		ft_pstr(2, "minishell: cd: OLDPWD not set\n");
		*error_flag = 1;
		return NULL;
	}
	return ft_strdup(var);
}

static char *handle_no_arg_env(t_env *env, int *error_flag) {
	char *var = find_env_value_env(env, "HOME");
	if (!var) {
		ft_pstr(2, "minishell: cd: HOME not set\n");
		*error_flag = 1;
		return NULL;
	}
	return ft_strdup(var);
}

char *handle_special_args_env(char **args, t_env *env, int *error_flag) {
	if (args[1] && ft_strcmp(args[1], "-") == 0)
		return handle_dash_arg_env(env, error_flag);
	if (args[1] && args[1][0] == '\0')
		return ft_strdup(".");
	if (!args[1])
		return handle_no_arg_env(env, error_flag);
	return NULL;
}
