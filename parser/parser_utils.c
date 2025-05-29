/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:38:12 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/29 15:17:26 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
