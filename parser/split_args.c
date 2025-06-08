/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:47:30 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 16:00:23 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_state(t_token_state *state, char *in, t_env *env, int lec)
{
	state->vars = init_vars();
	if (!state->vars)
		return (0);
	state->cap = ARGS_INIT_CAPACITY;
	state->args = malloc(sizeof(char *) * state->cap);
	if (!state->args)
	{
		free(state->vars);
		return (0);
	}
	state->input = in;
	state->envp = env;
	state->lec = lec;
	return (1);
}

int	handle_parse_error(t_token_state *state, int ret)
{
	if (ret == -2)
	{
		cleanup_partial_args(state->args, state->vars->j);
		free(state->args);
		free(state->vars);
		return (-2);
	}
	if (ret == -1)
	{
		free_and_return_null(state->vars, state->args);
		return (-1);
	}
	return (0);
}

void	mark_quoted_token(t_token_state *state, int was_quoted)
{
	char	*marked_token;

	if (was_quoted && state->vars->j > 0)
	{
		marked_token = ft_strjoin("QUOTED:", state->args[state->vars->j - 1]);
		free(state->args[state->vars->j - 1]);
		state->args[state->vars->j - 1] = marked_token;
	}
}

int	process_single_token(t_token_state *state)
{
	int	was_quoted;
	int	ret;

	was_quoted = is_token_quoted(state->input, state->vars->i);
	ret = parse_and_store_token(state);
	if (ret <= 0)
		return (ret);
	mark_quoted_token(state, was_quoted);
	return (ret);
}

char	**finalize_token_array(t_token_state *state)
{
	char	**result;

	state->args[state->vars->j] = NULL;
	result = state->args;
	free(state->vars);
	return (result);
}
