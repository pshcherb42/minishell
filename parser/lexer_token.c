/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:15:21 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 15:43:07 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	read_token(t_split_vars *vars, char *input)
{
	while (input[vars->i])
	{
		if (is_quote(input[vars->i]) && !vars->in_quote)
		{
			vars->in_quote = 1;
			vars->quote_char = input[vars->i++];
			continue ;
		}
		if (vars->in_quote && input[vars->i] == vars->quote_char)
		{
			vars->in_quote = 0;
			vars->i++;
			continue ;
		}
		if (input[vars->i] == ' ' && !vars->in_quote)
			break ;
		vars->i++;
	}
}

static int	handle_quote_char(t_split_vars *vars, char c)
{
	if (!vars->inside_quotes)
	{
		vars->inside_quotes = 1;
		vars->q_char = c;
		if (c == '\'')
			vars->was_single_quoted = 1;
		return (1);
	}
	else if (c == vars->q_char)
	{
		vars->inside_quotes = 0;
		return (1);
	}
	return (0);
}

int	copy_token(t_split_vars *vars, char *input)
{
	while (vars->m < vars->i)
	{
		if (input[vars->m] == '\\' || input[vars->m] == ';')
		{
			ft_printf("Unsupported character: %c\n", input[vars->m]);
			free(vars->raw_token);
			return (0);
		}
		else if (is_quote(input[vars->m]))
		{
			if (handle_quote_char(vars, input[vars->m]))
			{
				vars->m++;
				continue ;
			}
		}
		vars->raw_token[vars->k++] = input[vars->m++];
	}
	return (1);
}

int	init_token(t_split_vars *vars, int start)
{
	vars->len = vars->i - start;
	vars->raw_token = malloc(vars->len + 1);
	if (!vars->raw_token)
		return (0);
	vars->k = 0;
	vars->m = start;
	vars->inside_quotes = 0;
	vars->was_single_quoted = 0;
	vars->q_char = '\0';
	return (1);
}
