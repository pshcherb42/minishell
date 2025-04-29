/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:39:07 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/29 16:50:21 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_var_name(t_expand_ctx *ctx, int *i)
{
	char	*var;
	int		k;

	k = 0;
	var = malloc(256);
	allocation_error(var);
	while (ctx->input[*i]
		&& (ft_isalnum(ctx->input[*i]) || ctx->input[*i] == '_'))
		var[k++] = ctx->input[(*i)++];
	var[k] = '\0';
	return (var);
}

char	*find_env_value(t_expand_ctx *ctx, const char *var_name)
{
	int	m;
	int	len;

	if (!var_name[0])
		return (ft_strdup(""));
	len = ft_strlen(var_name);
	m = 0;
	while (ctx->envp[m])
	{
		if (!ft_strncmp(ctx->envp[m], var_name, len)
			&& ctx->envp[m][len] == '=')
			return (ft_strdup(ctx->envp[m] + len + 1));
		m++;
	}
	return (ft_strdup(""));
}

char	*get_env_value(t_expand_ctx *ctx, int *i)
{
	char	*var_name;
	char	*value;

	var_name = extract_var_name(ctx, i);
	value = find_env_value(ctx, var_name);
	free(var_name);
	return (value);
}
