/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:28 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/29 18:24:30 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static void		read_token(t_split_vars *vars, char *input)
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

char	**split_args(char *input, char **envp, int last_exit_code)
{
	char			**args;
	int				start;
	t_split_vars	*vars;

	vars = malloc(sizeof(t_split_vars));
	if (!vars)
		return (NULL);
	vars->quote_char = '\0';
	vars->in_quote = 0;
	vars->i = 0;
	vars->j = 0;
	args = malloc(sizeof(char *) * 100);
	if (!args)
		return (NULL);
	while (input[vars->i])
	{
		while (input[vars->i] == ' ' && !vars->in_quote)
			vars->i++;
		if (!input[vars->i])
			break ;
		start = vars->i;
		vars->len = 0;
		read_token(vars, input);
		vars->len = vars->i - start;
		vars->raw_token = malloc(vars->len + 1);
		if (!vars->raw_token)
			return (NULL);
		vars->k = 0;
		vars->m = start;
		vars->inside_quotes = 0;
		vars->q_char = '\0';
		while (vars->m < vars->i)
		{
			if (input[vars->m] == '\\' || input[vars->m] == ';')
			{
				ft_printf("Unsupported character: %c\n", input[vars->m]);
				free(vars->raw_token);
				return (NULL);
			}
			else if (is_quote(input[vars->m]))
			{
				if (!vars->inside_quotes)
				{
					vars->inside_quotes = 1;
					vars->q_char = input[vars->m++];
					continue ;
				}
				else if (input[vars->m] == vars->q_char)
				{
					vars->inside_quotes = 0;
					vars->m++;
					continue ;
				}
			}
			vars->raw_token[vars->k++] = input[vars->m++];
		}
		vars->raw_token[vars->k] = '\0';
		vars->expanded = vars->raw_token;
		ft_printf("TOKEN[%d]: \"%s\"\n", vars->j, vars->expanded);
		if (vars->raw_token[0] != '\'' && ft_strchr(vars->raw_token, '$'))
		{
			vars->expanded = expand_variables(vars->raw_token, envp, last_exit_code);
			free(vars->raw_token);
		}
		args[vars->j++] = vars->expanded;
	}
	args[vars->j] = NULL;
	return (args);
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
