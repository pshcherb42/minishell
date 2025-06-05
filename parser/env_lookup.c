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
	t_env *cur = ctx->envp;
	if (!var_name[0])
		return (ft_strdup(""));
	while (cur)
	{
		if (!ft_strcmp(cur->name, var_name))
			return (cur->value ? ft_strdup(cur->value) : ft_strdup(""));
		cur = cur->next;
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
