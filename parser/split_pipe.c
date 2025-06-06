/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:10:49 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 22:45:49 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**split_by_pipe(char *input)
{
    t_pipe_state	state;
    char			*segment;

    state.result = malloc(sizeof(char *) * 100);
    state.quote = '\0';
    state.i = 0;
    state.j = 0;
    state.start = 0;
    state.in_quote = 0;
    printf("DEBUG: split by pipes\n");
    while (input[state.i])
    {
        handle_quote(input[state.i], &state.quote, &state.in_quote);
        if (input[state.i] == '|' && !state.in_quote)
        {
			//printf("cucufu\n");
            segment = ft_strndup(input + state.start, state.i - state.start);
            state.result[state.j++] = segment;
            state.start = state.i + 1;
        }
        state.i++;
    }
    segment = ft_strndup(input + state.start, state.i - state.start);
    state.result[state.j++] = segment;
    state.result[state.j] = NULL;
    return (state.result);
}

/*char	**split_by_pipe(char *input)
{
	t_pipe_state	state;
	char			*segment;
	char			*quoted_segment;

	state.result = malloc(sizeof(char *) * 100);
	state.quote = '\0';
	state.i = 0;
	state.j = 0;
	state.start = 0;
	state.in_quote = 0;
	//printf("DEBUG: handling quotes\n");
	while (input[state.i])
	{
		handle_quote(input[state.i], &state.quote, &state.in_quote);
		if (input[state.i] == '|' && !state.in_quote)
		{
			//printf("DEBUG: handling pipes in quotes\n");
			segment = ft_strndup(input + state.start, state.i - state.start);
			if (state.in_quote)
            {
				printf("quoted: %s\n", quoted_segment);
                quoted_segment = ft_strjoin("QUOTED:", segment);
                free(segment);
                segment = quoted_segment;
            }
			state.result[state.j++] = segment;
			state.start = state.i + 1;
		}
		state.i++;
	}
	segment = ft_strndup(input + state.start, state.i - state.start);
	if (state.in_quote)
    {
        quoted_segment = ft_strjoin("QUOTED:", segment);
		printf("quoted: %s\n", quoted_segment);
        free(segment);
        segment = quoted_segment;
    }
	state.result[state.j++] = segment;
	state.result[state.j] = NULL;
	return (state.result);
}*/

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
