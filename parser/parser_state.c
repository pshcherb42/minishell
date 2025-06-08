/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:58:21 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/03 17:23:17 by akreise          ###   ########.fr       */
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

static int	count_consecutive_operators(const char *str, char op)
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

static int	check_invalid_op_seq(const char *segment)
{
    while (*segment)
    {
        if (*segment == '<')
        {
            int count = count_consecutive_operators(segment, '<');
            if (count >= 3) // <<<, <<<<, etc.
                return (1);
			if (count == 1 && *(segment + 1) == '>')
				return (1);
            segment += count;
        }
        else if (*segment == '>')
        {
            int count = count_consecutive_operators(segment, '>');
            if (count >= 3) // >>>, >>>>, etc.
                return (1);
			if (count == 1 && *(segment + 1) == '<')
				return (1);
            segment += count;
        }
        else
            segment++;
    }
    return (0);
}

static int	is_invalid_syntax(const char *segment)
{
	while (*segment && ft_isspace((unsigned char)*segment))
		segment++;
	if (*segment == ';' || *segment == '|')
		return (1);
	if (check_invalid_op_seq(segment))
		return (1);
	return (0);
}

static char	*find_invalid_token(const char *segment)
{
    while (*segment && ft_isspace((unsigned char)*segment))
        segment++;
        
    // Buscar el primer token inválido
    while (*segment)
    {
        if (*segment == '<')
        {
            int count = count_consecutive_operators(segment, '<');
            if (count >= 3)
			{
				if (count >= 4)
					return (ft_strdup("<<"));
				else
                	return (ft_strdup("<"));
			}
            if (*(segment + 1) == '>')
                return (ft_strdup("<"));
            segment += count;
        }
        else if (*segment == '>')
        {
            int count = count_consecutive_operators(segment, '>');
            if (count >= 3)
			{
				if (count >= 4)
					return (ft_strdup(">>"));
				else
                	return (ft_strdup(">"));
			} 
            if (*(segment + 1) == '<')
                return (ft_strdup(">"));
            segment += count;
        }
        else
            segment++;
    }
    return (ft_strdup("newline"));
}

int	process_segment(t_parse_state *state)
{
	char	*segment;
	t_cmd	*new_cmd;

	segment = state->segments[state->i];
	if (is_empty_or_spaces(segment))
	{
		ft_printf("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	else if (is_invalid_syntax(segment))
	{
		char *invalid_token = find_invalid_token(segment);
		ft_printf("minishell: syntax error near unexpected token `%s'\n",
			invalid_token);
		free(invalid_token);
		return (0);
	}
	new_cmd = parse_single_command(segment, state->env, state->last_exit_code);
	if (!new_cmd)
		return (0);
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
