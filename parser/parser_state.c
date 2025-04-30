/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:58:21 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 17:25:23 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_empty_or_spaces(const char *str)
{
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

int	process_segment(t_parse_state *state)
{
	char	*segment;
	t_cmd	*new_cmd;

	segment = state->segments[state->i];
	if (is_empty_or_spaces(segment))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	new_cmd = parse_single_command(segment, state->envp, state->last_exit_code);
	if (!new_cmd)
		return (1);
	if (!state->head)
		state->head = new_cmd;
	else
		state->tail->next = new_cmd;
	state->tail = new_cmd;
	return (1);
}

t_cmd	*build_command_list(t_parse_state *state)
{
	while (state->segments[state->i])
	{
		if (!process_segment(state))
		{
			while (state->segments[state->i])
				free(state->segments[state->i++]);
			return (NULL);
		}
		free(state->segments[state->i]);
		state->i++;
	}
	return (state->head);
}
