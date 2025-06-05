/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:54 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/03 19:24:57 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_escape_sequence(t_expand_ctx *ctx)
{
	if (ctx->input[ctx->i] != '\\' || !ctx->input[ctx->i + 1])
		return (0);
	if (ctx->input[ctx->i + 1] == '$')
		ctx->result[ctx->j++] = '$';
	else if (ctx->input[ctx->i + 1] == 'n')
		ctx->result[ctx->j++] = '\n';
	else if (ctx->input[ctx->i + 1] == 't')
		ctx->result[ctx->j++] = '\t';
	else if (ctx->input[ctx->i + 1] == '\\')
		ctx->result[ctx->j++] = '\\';
	else if (ctx->input[ctx->i + 1] == 'r')
		ctx->result[ctx->j++] = '\r';
	else if (ctx->input[ctx->i + 1] == '"')
		ctx->result[ctx->j++] = '"';
	else
		ctx->result[ctx->j++] = ctx->input[ctx->i + 1];
	ctx->i += 2;
	return (1);
}

static void	init_expand_context(t_expand_ctx *ctx, const char *input,
								t_env *env, int lec)
{
	ctx->i = 0;
	ctx->j = 0;
	ctx->input = input;
	ctx->envp = env;
	ctx->last_exit_code = lec;
	ctx->result = malloc(4096);
	allocation_error(ctx->result);
}

static void	process_character(t_expand_ctx *ctx)
{
	if (handle_escape_sequence(ctx))
		return ;
	else if (ctx->input[ctx->i] == '$')
	{
		ctx->i++;
		if (ctx->input[ctx->i] == '?')
			handle_exit_code(ctx);
		else
			handle_env_var(ctx);
	}
	else
		ctx->result[ctx->j++] = ctx->input[ctx->i++];
}

char	*expand_variables(const char *input, t_env *env, int lec)
{
	t_expand_ctx	ctx;

	init_expand_context(&ctx, input, env, lec);
	while (input[ctx.i])
		process_character(&ctx);
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}
