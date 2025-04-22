/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:28 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/22 15:07:25 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/*int	update_quote_status(char c, t_lexer_ctx *ctx)
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
}*/

static char	*extract_token(const char *input, t_lexer_ctx *ctx,
							char **envp, int last_exit_code)
{
	printf("Extracting token from [%.*s]\n", ctx->len, input + ctx->start);
	printf("ctx->len = %d, start = %d, i = %d\n", ctx->len, ctx->start, ctx->i);

	ctx->raw_token = malloc(ctx->len + 1);
	if (!ctx->raw_token)
		return (NULL);
	ctx->k = 0;
	ctx->m = ctx->start;
	ctx->inside_quotes = 0;
	ctx->quote_char = '\0';
	while (ctx->m < ctx->i)
	{
		if (handle_escape(input, ctx))
			continue ;
		if (handle_quote_chars(input, ctx))
			continue ;
		ctx->raw_token[ctx->k++] = input[ctx->m++];
	}
	ctx->raw_token[ctx->k] = '\0';
	if (ctx->quote_char != '\'')
	{
		ctx->expanded = expand_variables(ctx->raw_token, envp, last_exit_code);
		free(ctx->raw_token);
		return (ctx->expanded);
	}
	return (ctx->raw_token);
}

char	**split_args(char *input, char **envp, int last_exit_code)
{
	char		**args;
	t_lexer_ctx	ctx;

	args = malloc(sizeof(char *) * 100);
	if (!args)
		return (NULL);
	init_lexer_ctx(&ctx);
	while (input[ctx.i])
	{
		if (!parse_token_boundaries(input, &ctx))
			break ;
		ctx.expanded = extract_token(input, &ctx, envp, last_exit_code);
		if (!ctx.expanded)
			return (NULL);
		args[ctx.j++] = ctx.expanded;
	}
	args[ctx.j] = NULL;
	return (args);
}
