/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:15:21 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/01 17:07:06 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

static void	handle_quotes(t_split_vars *vars, char c)
{
	if (is_quote(c) && !vars->in_quote)
	{
		vars->in_quote = 1;
		vars->quote_char = c;
	}
	else if (vars->in_quote && c == vars->quote_char)
		vars->in_quote = 0;
}

static int	handle_operator(t_split_vars *vars, char *input, int start)
{
	if (vars->i == start)
	{
		if ((input[vars->i] == '<' || input[vars->i] == '>')
			&& input[vars->i + 1] && input[vars->i] == input[vars->i + 1])
			vars->i += 2;
		else
			vars->i += 1;
		return (1);
	}
	return (0);
}

void	read_token(t_split_vars *vars, char *input, int start)
{
	while (input[vars->i])
	{
		handle_quotes(vars, input[vars->i]);
		if (!vars->in_quote && is_operator(input[vars->i]))
		{
			if (handle_operator(vars, input, start))
				break ;
			break ;
		}
		if (!vars->in_quote && input[vars->i] == ' ')
			break ;
		vars->i++;
	}
}
