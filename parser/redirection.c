/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:03:08 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/08 16:06:47 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

static bool	is_quoted(const char *token)
{
	if (!token)
		return (false);
	return (ft_strncmp(token, "QUOTED:", 7) == 0);
}

static int	handle_output_redirect(t_cmd *cmd, char **tokens, int i)
{
	int	fd;

	if (!tokens[i + 1])
	{
		ft_printf("minishell: ");
		ft_printf("syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(tokens[++i]);
	if (!cmd->outfile)
	{
		perror("minishell");
		return (-1);
	}
	cmd->append = 0;
	fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (i + 1);
}

static int	handle_append_redirect(t_cmd *cmd, char **tokens, int i)
{
	int	fd;

	if (!tokens[i + 1])
	{
		ft_printf("minishell: ");
		ft_printf("syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(tokens[++i]);
	if (!cmd->outfile)
	{
		perror("minishell");
		return (-1);
	}
	cmd->append = 1;
	fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (i + 1);
}

static int	handle_heredoc_redirect(t_cmd *cmd, char **tokens, int i)
{
	char	*temp_infile;

	if (!tokens[i + 1])
	{
		ft_printf("minishell: ");
		ft_printf("syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->heredoc_file)
	{
		cleanup_temp_file(cmd->heredoc_file);
		cmd->heredoc_file = NULL;
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
	free(temp_infile);
	cmd->heredoc = 1;
	return (i + 1);
}

int	handle_redirection(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i])
		return (i);
	if (is_quoted(tokens[i]))
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
