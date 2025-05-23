/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:37:31 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/23 18:22:49 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_exit_code(t_expand_ctx *ctx)
{
	char	*itoa_str;
	int		k;

	k = 0;
	itoa_str = ft_itoa(ctx->last_exit_code);
	allocation_error(itoa_str);
	while (itoa_str[k])
		ctx->result[ctx->j++] = itoa_str[k++];
	free(itoa_str);
	ctx->i++;
}

void	handle_env_var(t_expand_ctx *ctx)
{
	char	*val;
	int		v;

	if (!(ft_isalnum(ctx->input[ctx->i]) || ctx->input[ctx->i] == '_'))
	{
		ctx->result[ctx->j++] = '$';
		if (ctx->input[ctx->i])
			ctx->result[ctx->j++] = ctx->input[ctx->i++];
		return ;
	}
	val = get_env_value(ctx, &ctx->i);
	v = 0;
	while (val[v])
		ctx->result[ctx->j++] = val[v++];
	free(val);
}

void	allocation_error(char *str)
{
	if (!str)
	{
		ft_printf("Memory allocation failed.\n");
		exit(1);
	}
}
