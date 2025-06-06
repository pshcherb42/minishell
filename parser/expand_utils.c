/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:37:31 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 22:57:46 by pshcherb         ###   ########.fr       */
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

	printf("DEBUG: Expanding variable at position %d\n", ctx->i);
	if (!(ft_isalpha(ctx->input[ctx->i]) || ctx->input[ctx->i] == '_'))
	{
		if (ft_isdigit(ctx->input[ctx->i]))
		{
			printf("DEBUG: Variable is a digit, treating as literal: %c\n", ctx->input[ctx->i]);
			ctx->result[ctx->j++] = '$';
			while (ft_isalnum(ctx->input[ctx->i]))
				ctx->result[ctx->j++] = ctx->input[ctx->i++];
			return ;
		}
		printf("DEBUG: Invalid variable, treating as literal: %c\n", ctx->input[ctx->i]);
		ctx->result[ctx->j++] = '$';
		if (ctx->input[ctx->i])
			ctx->result[ctx->j++] = ctx->input[ctx->i++];
		return ;
	}
	val = get_env_value(ctx, &ctx->i);
	printf("DEBUG: Variable expanded to: %s\n", val ? val : "(null)");
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
