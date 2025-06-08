/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:10:49 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 17:05:28 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_pipe_state(t_pipe_state *state)
{
	state->result = malloc(sizeof(char *) * 100);
	if (!state->result)
		return (0);
	state->quote = '\0';
	state->i = 0;
	state->j = 0;
	state->start = 0;
	state->in_quote = 0;
	return (1);
}

char	*apply_quote_prefix_if_needed(char *segment, int in_quote)
{
	char	*quoted_segment;

	if (in_quote != '\0')
	{
		quoted_segment = ft_strjoin("QUOTED:", segment);
		free(segment);
		return (quoted_segment);
	}
	return (segment);
}

void	process_pipe_found(char *input, t_pipe_state *state)
{
	char	*segment;

	segment = extract_segment(input, state->start, state->i);
	segment = apply_quote_prefix_if_needed(segment, state->in_quote);
	store_segment(state, segment);
	state->start = state->i + 1;
}

void	finalize_result(char *input, t_pipe_state *state)
{
	char	*segment;

	segment = extract_segment(input, state->start, state->i);
	segment = apply_quote_prefix_if_needed(segment, state->in_quote);
	store_segment(state, segment);
	state->result[state->j] = NULL;
}

char	**split_by_pipe(char *input)
{
	t_pipe_state	state;

	if (!init_pipe_state(&state))
		return (NULL);
	while (input[state.i])
	{
		handle_quote(input[state.i], &state.quote, &state.in_quote);
		if (input[state.i] == '|' && !state.in_quote)
			process_pipe_found(input, &state);
		state.i++;
	}
	finalize_result(input, &state);
	return (state.result);
}
