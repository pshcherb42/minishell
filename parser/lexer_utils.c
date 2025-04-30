/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:18:45 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 15:44:19 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

char	**free_and_return_null(t_split_vars *vars, char **args)
{
	free(vars);
	free(args);
	return (NULL);
}

int	validate_quotes(const char *input)
{
	char	quote;
	int		i;

	quote = '\0';
	i = 0;
	while (input[i])
	{
		if (is_quote(input[i]))
		{
			if (quote == '\0')
				quote = input[i];
			else if (input[i] == quote)
				quote = '\0';
		}
		i++;
	}
	return (quote == '\0');
}

t_split_vars	*init_vars(void)
{
	t_split_vars	*vars;

	vars = malloc(sizeof(t_split_vars));
	if (!vars)
		return (NULL);
	vars->quote_char = '\0';
	vars->in_quote = 0;
	vars->i = 0;
	vars->j = 0;
	return (vars);
}
