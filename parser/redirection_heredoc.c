/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:32:05 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 21:37:03 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_previous_input(t_cmd *cmd)
{
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->heredoc_file)
	{
		cleanup_temp_file(cmd->heredoc_file);
		cmd->heredoc_file = NULL;
	}
	if (cmd->heredoc && cmd->heredoc_fd >= 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	cmd->heredoc = 0;
}

int	handle_input_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	cleanup_previous_input(cmd);
	cmd->infile = ft_strdup(tokens[++i]);
	return (i + 1);
}
