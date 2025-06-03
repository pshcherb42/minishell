/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:03:08 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/03 14:49:31 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_output_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(tokens[++i]);
	cmd->append = 0;
	return (i + 1);
}

static int	handle_append_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(tokens[++i]);
	cmd->append = 1;
	return (i + 1);
}

static int	handle_input_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(tokens[++i]);
	if (cmd->heredoc && cmd->heredoc_fd >= 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	cmd->heredoc = 0;
	return (i + 1);
}

static int	handle_heredoc_redirect(t_cmd *cmd, char **tokens, int i)
{
	char	*temp_infile;
	
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	temp_infile = ft_strdup(tokens[++i]);
	if (!temp_infile)
		return (-1);
	if (handle_here_fork(temp_infile, &cmd->heredoc_file) == -1)
	{
		free(temp_infile);
		cmd->heredoc_interrupted = 1;
		return (-2);
	}
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->heredoc_file && cmd->heredoc)
		cleanup_temp_file(cmd->heredoc_file);
	cmd->infile = temp_infile;
	cmd->heredoc = 1;
	return (i + 1);
}

int	handle_redirection(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i])
		return (i);
	if (!ft_strcmp(tokens[i], ">"))
		return (handle_output_redirect(cmd, tokens, i));
	else if (!ft_strcmp(tokens[i], ">>"))
		return (handle_append_redirect(cmd, tokens, i));
	else if (!ft_strcmp(tokens[i], "<"))
		return (handle_input_redirect(cmd, tokens, i));
	else if (!ft_strcmp(tokens[i], "<<"))
		return (handle_heredoc_redirect(cmd, tokens, i));
	return (i);
}
