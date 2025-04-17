/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:16 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/17 14:37:31 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_segment(char *seg, t_parse_ctx *ctx)
{
	t_cmd	*new_cmd;

	new_cmd = parse_single_command(seg, ctx->envp, ctx->code);
	if (!new_cmd)
		return ;
	if (!*(ctx->head))
		*(ctx->head) = new_cmd;
	else
		(*(ctx->tail))->next = new_cmd;
	*(ctx->tail) = new_cmd;
}

t_cmd	*parse_input(char *input, char **envp, int last_exit_code)
{
	char		**segments;
	t_cmd		*head;
	t_cmd		*tail;
	t_parse_ctx	ctx;
	int			i;

	segments = split_by_pipe(input);
	head = NULL;
	tail = NULL;
	ctx.head = &head;
	ctx.tail = &tail;
	ctx.envp = envp;
	ctx.code = last_exit_code;
	i = 0;
	while (segments[i])
	{
		process_segment(segments[i], &ctx);
		free(segments[i]);
		i++;
	}
	free(segments);
	return (head);
}
