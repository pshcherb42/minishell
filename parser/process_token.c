/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:32:36 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 15:40:37 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_redirection_result(t_cmd *cmd, int res, int j)
{
	if (res == -1)
	{
		cleanup_args_on_error(cmd, j);
		return (-1);
	}
	if (res == -2)
	{
		cleanup_args_on_error(cmd, j);
		cmd->heredoc_interrupted = 1;
		return (-2);
	}
	return (0);
}

static int	process_as_argument(t_cmd *cmd, char **tokens, int i, int *j)
{
	if (!add_arg(cmd, tokens[i], j))
	{
		cmd->args[*j] = NULL;
		return (-1);
	}
	return (0);
}

static void	update_token_index(int res, int *i)
{
	if (res > *i)
		*i = res;
	else
		(*i)++;
}

int	process_token(t_cmd *cmd, char **tokens, int *i, int *j)
{
	int	res;
	int	status;

	res = handle_redirection(cmd, tokens, *i);
	status = handle_redirection_result(cmd, res, *j);
	if (status != 0)
		return (status);
	if (res == *i)
	{
		status = process_as_argument(cmd, tokens, *i, j);
		if (status != 0)
			return (status);
	}
	update_token_index(res, i);
	return (0);
}
