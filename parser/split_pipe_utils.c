/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:55:01 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 17:06:20 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quote(char c, char *quote, int *in_quote)
{
	if ((c == '\'') || c == '"')
	{
		if (!*in_quote)
		{
			*quote = c;
			*in_quote = 1;
		}
		else if (*quote == c)
		{
			*in_quote = 0;
			*quote = '\0';
		}
	}
}

char	*extract_segment(char *input, int start, int end)
{
	return (ft_strndup(input + start, end - start));
}

void	store_segment(t_pipe_state *state, char *segment)
{
	state->result[state->j++] = segment;
}
