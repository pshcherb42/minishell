/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add_eq.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/05/26 20:43:10 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_old_val(const char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (ft_strdup(""));
	return (ft_strdup(eq + 1));
}

static char	*join_var_eq_val(char *var_name, char *value)
{
	char	*temp;
	char	*final;

	temp = ft_strjoin(var_name, "=");
	if (!temp)
		return (NULL);
	final = ft_strjoin_free(temp, value);
	return (final);
}

static	int	upd_existing_var(char ***envp, int idx, char *arg, char *var_name)
{
	char	*append;
	char	*old;
	char	*joined;
	char	*final;

	append = prep_joined(arg, var_name);
	old = get_old_val((*envp)[idx]);
	if (!append || !old)
		return (free(var_name), free(append), free(old), 1);
	joined = ft_strjoin(old, append);
	free(old);
	free(append);
	if (!joined)
		return (free(var_name), 1);
	final = join_var_eq_val(var_name, joined);
	free(joined);
	if (!final)
		return (free(var_name), 1);
	free((*envp)[idx]);
	(*envp)[idx] = final;
	return (free(var_name), 0);
}

int	add_new(char ***envp, const char *arg, const char *var_name, int name_len)
{
	char	*val;
	char	*joined;
	int		res;

	val = ft_strdup(arg + name_len + 2);
	if (!val)
		return (free((char *)var_name), 1);
	joined = join_var_eq_val((char *)var_name, val);
	free(val);
	if (!joined)
		return (free((char *)var_name), 1);
	res = add_env_var(envp, joined);
	free(joined);
	free((char *)var_name);
	return (res);
}

int	ft_add_eq(char ***envp, char *arg, int name_len)
{
	int		i;
	char	*var_name;

	var_name = ft_substr(arg, 0, name_len);
	if (!var_name)
		return (1);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], var_name, name_len) &&
			(*envp)[i][name_len] == '=')
			return (upd_existing_var(envp, i, arg, var_name));
		i++;
	}
	return (add_new(envp, arg, var_name, name_len));
}
