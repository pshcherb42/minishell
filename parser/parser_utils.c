/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:38:12 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/20 16:41:21 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*char	*read_continued_input(const char *input)
{
	char	*full_input;
	char	*next_line;
	char	*tmp;

	if (is_empty_or_spaces(input))
		return (NULL);
	full_input = ft_strdup(input);
	while (is_trailing_pipe(full_input))
	{
		next_line = readline("> ");
		if (!next_line)
			break ;
		tmp = full_input;
		full_input = ft_strjoin(tmp, next_line);
		free(tmp);
		free(next_line);
	}
	return (full_input);
}*/

static int	is_outside_quotes(const char *input, int index)
{
	int		in_quote;
	char	q;
	int		i;

	in_quote = 0;
	q = '\0';
	i = 0;
	while (i < index)
	{
		if (is_quote(input[i]))
		{
			if (!in_quote)
			{
				in_quote = 1;
				q = input[i];
			}
			else if (q == input[i])
				in_quote = 0;
		}
		i++;
	}
	return (in_quote == 0);
}

int	is_trailing_pipe(const char *input)
{
	int	i;

	if (!validate_quotes(input))
		return (0);
	i = ft_strlen(input) - 1;
	while (i >= 0 && ft_isspace((unsigned char)input[i]))
		i--;
	return (i >= 0 && input[i] == '|' && is_outside_quotes(input, i));
}
