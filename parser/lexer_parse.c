/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:20:20 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 15:58:04 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_token(t_split_vars *vars, char *in, t_env *env, int lec)
{
	int		start;

	start = vars->i;
	vars->len = 0;
	read_token(vars, in, start);
	if (!init_token(vars, start))
		return (0);
	if (!copy_token(vars, in))
		return (0);
	vars->raw_token[vars->k] = '\0';
	vars->expanded = vars->raw_token;
	expand_token(vars, env, lec);
	if (vars->expanded == NULL)
		return (0);
	return (1);
}

int	parse_and_store_token(t_token_state *state)
{
	while (state->input[state->vars->i] == ' ' && !state->vars->in_quote)
		state->vars->i++;
	if (!state->input[state->vars->i])
		return (0);
	if (!parse_token(state->vars, state->input, state->envp, state->lec))
	{
		if (state->vars->syntax_error)
			return (-2);
		return (-1);
	}
	if (state->vars->j >= state->cap - 1)
	{
		state->args = grow_args_array(state->args, state->vars->j, &state->cap);
		if (!state->args)
			return (-1);
	}
	state->args[state->vars->j++] = state->vars->expanded;
	return (1);
}

int	is_token_quoted(char *input, int pos)
{
	while (input[pos] == ' ')
		pos++;
	return (input[pos] == '"' || input[pos] == '\'');
}

char	**split_args(char *input, t_env *envp, int last_exit_code)
{
	t_token_state	state;
	int				ret;

	if (!init_state(&state, input, envp, last_exit_code))
		return (NULL);
	while (input[state.vars->i])
	{
		ret = process_single_token(&state);
		if (ret <= 0)
		{
			if (ret < 0)
				handle_parse_error(&state, ret);
			break ;
		}
	}
	return (finalize_token_array(&state));
}

void	cleanup_partial_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (args[i])
		{
			free(args[i]);
			args[i] = NULL;
		}
		i++;
	}
}
