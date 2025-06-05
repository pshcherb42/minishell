/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add_eq.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 16:53:28 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_old_val(t_env *env_node)
{
	if (!env_node || !env_node->value)
		return (ft_strdup(""));
	return (ft_strdup(env_node->value));
}

static int	upd_existing_var(t_env **env, t_env *existing_node, char *arg,
	char *var_name)
{
	char	*append;
	char	*old;
	char	*joined;
	int		result;

	append = arg + ft_strlen(var_name) + 2;
	old = get_old_val(existing_node);
	if (!old)
		return (free(var_name), 1);
	joined = ft_strjoin(old, append);
	free(old);
	if (!joined)
		return (free(var_name), 1);
	result = env_list_set(env, var_name, joined);
	free(joined);
	return (free(var_name), result);
}

static int	add_new(t_env **env, const char *arg, const char *var_name,
	int name_len)
{
	char	*val;
	int		res;

	val = ft_strdup(arg + name_len + 2);
	if (!val)
		return (free((char *)var_name), 1);
	res = env_list_set(env, (char *)var_name, val);
	free(val);
	free((char *)var_name);
	return (res);
}

int	ft_add_eq(t_env **env, char *arg, int name_len)
{
	char	*var_name;
	t_env	*existing;

	var_name = strndup(arg, name_len);
	if (!var_name)
		return (1);
	existing = env_list_find(*env, var_name);
	if (existing)
		return (upd_existing_var(env, existing, arg, var_name));
	return (add_new(env, arg, var_name, name_len));
}
