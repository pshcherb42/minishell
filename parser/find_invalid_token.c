/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_invalid_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:17:17 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 16:26:03 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*check_input_redirection(const char **segment)
{
	int	count;

	count = count_consecutive_operators(*segment, '<');
	if (count >= 3)
	{
		if (count >= 4)
			return (ft_strdup("<<"));
		else
			return (ft_strdup("<"));
	}
	if (*(*segment + 1) == '>')
		return (ft_strdup("<"));
	*segment += count;
	return (NULL);
}

static char	*check_output_redirection(const char **segment)
{
	int	count;

	count = count_consecutive_operators(*segment, '>');
	if (count >= 3)
	{
		if (count >= 4)
			return (ft_strdup(">>"));
		else
			return (ft_strdup(">"));
	}
	if (*(*segment + 1) == '<')
		return (ft_strdup(">"));
	*segment += count;
	return (NULL);
}

static const char	*skip_whitespace(const char *segment)
{
	while (*segment && ft_isspace((unsigned char)*segment))
		segment++;
	return (segment);
}

char	*find_invalid_token(const char *segment)
{
	char	*invalid_token;

	segment = skip_whitespace(segment);
	while (*segment)
	{
		if (*segment == '<')
		{
			invalid_token = check_input_redirection(&segment);
			if (invalid_token)
				return (invalid_token);
		}
		else if (*segment == '>')
		{
			invalid_token = check_output_redirection(&segment);
			if (invalid_token)
				return (invalid_token);
		}
		else
			segment++;
	}
	return (ft_strdup("newline"));
}
