/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:03:54 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/01 17:17:01 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_quote_char(t_split_vars *vars, char c)
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
