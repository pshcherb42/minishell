/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:28 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/25 15:12:05 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

char	**split_args(char *input, char **envp, int last_exit_code)
{
	char		**args;
	char		quote_char;
	int			in_quote;
	int			i;
	int			j;
	int			start;
	int			len;
	char		*raw_token;
	int			k;
	int			m;
	int			inside_quotes;
	char		*expanded;
	char		q_char;

	quote_char = '\0';
	in_quote = 0;
	i = 0;
	j = 0;
	args = malloc(sizeof(char *) * 100);
	if (!args)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ' && !in_quote)
			i++;
		if (!input[i])
			break ;
		start = i;
		len = 0;
		while (input[i])
		{
			if (is_quote(input[i]))
			{
				if (!in_quote)
				{
					in_quote = 1;
					quote_char = input[i++];
					continue ;
				}
				else if (input[i] == quote_char)
				{
					in_quote = 0;
					i++;
					continue ;
				}
				else
				{
					i++;
					continue ;
				}
			}
			if (input[i] == ' ' && !in_quote)
				break ;
			i++;
		}
		len = i - start;
		raw_token = malloc(len + 1);
		if (!raw_token)
			return (NULL);
		k = 0;
		m = start;
		inside_quotes = 0;
		q_char = '\0';
		while (m < i)
		{
			/*if (input[m] == '\\' && inside_quotes && q_char == '"'
				&& (input[m + 1] == '"' || input[m + 1] == '\\' || input[m + 1] == '$'))
			{
				// escapamos ", \, o $
				raw_token[k++] = input[m + 1];
				m += 2;
			}*/
			if (input[m] == '\\' || input[m] == ';')
			{
				ft_printf("Unsupported character: %c\n", input[m]);
				free(raw_token);
				/*while (j > 0)
					free(args[--j]);
				free(args);*/
				return (NULL);
			}
			else if (is_quote(input[m]))
			{
				if (!inside_quotes)
				{
					inside_quotes = 1;
					q_char = input[m++];
					continue ;
				}
				else if (input[m] == q_char)
				{
					inside_quotes = 0;
					m++;
					continue ;
				}
			}
			raw_token[k++] = input[m++];
		}
		raw_token[k] = '\0';
		expanded = raw_token;
		//ft_printf("TOKEN[%d]: \"%s\"\n", j, expanded);
		if (raw_token[0] != '\'' && ft_strchr(raw_token, '$'))
		{
			expanded = expand_variables(raw_token, envp, last_exit_code);
			free(raw_token);
		}
		args[j++] = expanded;
	}
	args[j] = NULL;
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
