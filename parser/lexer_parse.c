/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:20:20 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 21:36:46 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_token(t_split_vars *vars, char *in, t_env *env, int lec)
{
	int		start;

	start = vars->i;
	vars->len = 0;
	read_token(vars, in, start);
	/*if (vars->syntax_error)
		return (0);*/
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

static int	parse_and_store_token(t_token_state *state)
{
	while (state->input[state->vars->i] == ' ' && !state->vars->in_quote)
		state->vars->i++;
	if (!state->input[state->vars->i])
		return (0);
	//printf("DEBUG: parsing token\n");
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

static int	is_token_quoted(char *input, int pos)
{
    while (input[pos] == ' ')
        pos++;
    return (input[pos] == '"' || input[pos] == '\'');
}

char	**split_args(char *input, t_env *envp, int last_exit_code)
{
	t_token_state	state;
	int				ret;
	char			*marked_token;

	state.vars = init_vars();
	if (!state.vars)
		return (NULL);
	state.cap = ARGS_INIT_CAPACITY;
	state.args = malloc(sizeof(char *) * state.cap);
	if (!state.args)
		return (NULL);
	state.input = input;
	state.envp = envp;
	state.lec = last_exit_code;
	//printf("DEBUG: preparing to parse tokens\n");
	while (input[state.vars->i])
	{
		int was_quoted = is_token_quoted(input, state.vars->i);
		ret = parse_and_store_token(&state);
		if (ret == -2)
		{
			cleanup_partial_args(state.args, state.vars->j);
			free(state.args);
			free(state.vars);
			return (NULL);
		}
		if (ret == -1)
			return (free_and_return_null(state.vars, state.args));
		if (ret == 0)
			break ;
		if (was_quoted && state.vars->j > 0)
    	{
        	marked_token = ft_strjoin("QUOTED:", state.args[state.vars->j - 1]);
        	free(state.args[state.vars->j - 1]);
        	state.args[state.vars->j - 1] = marked_token;
    	}
	}
	state.args[state.vars->j] = NULL;
	free(state.vars);
	return (state.args);
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
