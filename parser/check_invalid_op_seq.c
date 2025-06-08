/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_invalid_op_seq.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:42:44 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 16:47:00 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_input_operator_sequence(const char **segment)
{
	int	count;

	count = count_consecutive_operators(*segment, '<');
	if (count >= 3)
		return (1);
	if (count == 1 && *(*segment + 1) == '>')
		return (1);
	*segment += count;
	return (0);
}

static int	check_output_operator_sequence(const char **segment)
{
	int	count;

	count = count_consecutive_operators(*segment, '>');
	if (count >= 3)
		return (1);
	if (count == 1 && *(*segment + 1) == '<')
		return (1);
	*segment += count;
	return (0);
}

int	check_invalid_op_seq(const char *segment)
{
	while (*segment)
	{
		if (*segment == '<')
		{
			if (check_input_operator_sequence(&segment))
				return (1);
		}
		else if (*segment == '>')
		{
			if (check_output_operator_sequence(&segment))
				return (1);
		}
		else
			segment++;
	}
	return (0);
}
