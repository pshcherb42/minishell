/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:53:42 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/22 10:51:12 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_quote_status(char c, t_lexer_ctx *ctx)
{
	if ((c == '\'' || c == '"'))
	{
		if (!ctx->in_quote)
		{
			ctx->in_quote = 1;
			ctx->quote_char = c;
		}
		else if (ctx->quote_char == c)
		{
			ctx->in_quote = 0;
			ctx->quote_char = '\0';
		}
		return (1);
	}
	return (0);
}

static void	fill_segments(char **result, char *input)
{
	t_lexer_ctx	ctx;

	ctx.i = 0;
	ctx.j = 0;
	ctx.in_quote = 0;
	while (input[ctx.i])
	{
		update_quote_status(input[ctx.i], &ctx);
		if (input[ctx.i] == '|' && !ctx.in_quote)
		{
			result[ctx.j++] = strndup(input + ctx.start, ctx.i - ctx.start);
			ctx.start = ctx.i + 1;
		}
		ctx.i++;
	}
	result[ctx.j++] = strndup(input + ctx.start, ctx.i - ctx.start);
	result[ctx.j] = NULL;
}

char	**split_by_pipe(char *input)
{
	char	**result;

	result = malloc(sizeof(char *) * 100);
	if (!result)
		return (NULL);
	fill_segments(result, input);
	return (result);
}
