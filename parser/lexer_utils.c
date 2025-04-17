/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:55:57 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/17 16:46:19 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_lexer_ctx(t_lexer_ctx *ctx)
{
	ctx->i = 0;
	ctx->j = 0;
	ctx->quote_char = '\0';
	ctx->in_quote = 0;
}

int	parse_token_boundaries(const char *input, t_lexer_ctx *ctx)
{
	while (input[ctx->i] == ' ' && !ctx->in_quote)
		ctx->i++;
	if (!input[ctx->i])
		return (0);
	ctx->start = ctx->i;
	while (input[ctx->i])
	{
		if (update_quote_status(input[ctx->i], ctx))
		{
			ctx->i++;
			continue ;
		}
		if (input[ctx->i] == ' ' && !ctx->in_quote)
			break ;
		ctx->i++;
	}
	ctx->len = ctx->i - ctx->start;
	return (1);
}

int	validate_quotes(const char *input)
{
	char	quote;
	int		i;

	quote = '\0';
	i = 0;
	while (input[i])
	{
		if (is_quote(input[i]))
		{
			if (quote == '\0')
				quote = input[i];
			else if (input[i] == quote)
				quote = '\0';
		}
		i++;
	}
	return (quote == '\0');
}

int     handle_escape(const char *input, t_lexer_ctx *ctx)
{
    if (input[ctx->m] == '\\'
        && ctx->inside_quotes
        && ctx->q_char == '"'
        && (input[ctx->m + 1] == '"' || input[ctx->m + 1] == '\\' || input[ctx->m + 1] == '$'))
    {
        ctx->raw_token[ctx->k++] = input[ctx->m + 1];
        ctx->m += 2;
        return (1);
    }
    return (0);
}

int     handle_quote_chars(const char *input, t_lexer_ctx *ctx)
{
    if (input[ctx->m] == '\'' || input[ctx->m] == '"')
    {
        if (!ctx->inside_quotes)
            ctx->q_char = input[ctx->m++];
        else if (input[ctx->m] == ctx->q_char)
        {
            ctx->inside_quotes = 0;
            ctx->m++;
        }
        else
            ctx->raw_token[ctx->k++] = input[ctx->m++];
        return (1);
    }
    return (0);
}