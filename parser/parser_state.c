/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:58:21 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 16:43:10 by pshcherb         ###   ########.fr       */
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

int	count_consecutive_operators(const char *str, char op)
{
	int	count;

	count = 0;
	while (*str == op)
	{
		count++;
		str++;
	}
	return (count);
}

int	is_invalid_syntax(const char *segment)
{
	while (*segment && ft_isspace((unsigned char)*segment))
		segment++;
	if (*segment == ';' || *segment == '|')
		return (1);
	if (check_invalid_op_seq(segment))
		return (1);
	return (0);
}

t_cmd	*build_command_list(t_parse_state *state)
{
	while (state->segments[state->i])
	{
		if (!process_segment(state))
		{
			if (state->head)
			{
				free_cmd_list(state->head);
				state->head = NULL;
			}
			while (state->segments[state->i])
				free(state->segments[state->i++]);
			return (NULL);
		}
		free(state->segments[state->i]);
		state->i++;
	}
	return (state->head);
}
