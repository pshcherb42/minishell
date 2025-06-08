/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_segment.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:27:13 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 16:40:56 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_empty_segment(void)
{
	ft_printf("minishell: syntax error near unexpected token `|'\n");
	return (0);
}

static int	handle_invalid_syntax(char *segment)
{
	char	*invalid_token;

	invalid_token = find_invalid_token(segment);
	ft_printf("minishell: syntax error near unexpected token `%s'\n",
		invalid_token);
	free(invalid_token);
	return (0);
}

static int	validate_segment(char *segment)
{
	if (is_empty_or_spaces(segment))
		return (handle_empty_segment());
	else if (is_invalid_syntax(segment))
		return (handle_invalid_syntax(segment));
	return (1);
}

static void	add_command_to_list(t_parse_state *state, t_cmd *new_cmd)
{
	if (!state->head)
		state->head = new_cmd;
	else
		state->tail->next = new_cmd;
	state->tail = new_cmd;
}

int	process_segment(t_parse_state *state)
{
	char	*segment;
	t_cmd	*new_cmd;
	int		validation_result;

	segment = state->segments[state->i];
	validation_result = validate_segment(segment);
	if (validation_result != 1)
		return (validation_result);
	new_cmd = parse_single_command(segment, state->env, state->last_exit_code);
	if (!new_cmd)
		return (0);
	add_command_to_list(state, new_cmd);
	return (1);
}
