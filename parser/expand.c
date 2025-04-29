/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:54 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/29 16:46:02 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	t_expand_ctx	ctx;

	ctx.i = 0;
	ctx.j = 0;
	ctx.input = input;
	ctx.envp = envp;
	ctx.last_exit_code = last_exit_code;
	ctx.result = malloc(4096);
	allocation_error(ctx.result);
	while (input[ctx.i])
	{
		if (input[ctx.i] == '$')
		{
			ctx.i++;
			if (input[ctx.i] == '?')
				handle_exit_code(&ctx);
			else
				handle_env_var(&ctx);
		}
		else
			ctx.result[ctx.j++] = input[ctx.i++];
	}
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}
