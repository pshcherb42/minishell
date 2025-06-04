/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/04 15:10:33 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_infile(t_cmd *cmd)
{
	int	fd;

	if (cmd->heredoc && cmd->heredoc_file)
	{
		fd = open(cmd->heredoc_file, O_RDONLY);
		if (fd < 0)
		{
			perror("heredoc temp file");
			return (0);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		return (1);
	}
	if (!cmd->infile)
		return (1);
	fd = open(cmd->infile, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

static int	open_outfile(t_cmd *cmd)
{
	int	fd;

	if (!cmd->outfile)
		return (1);
	if (cmd->append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

static int	validate_input_files(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			ft_printf("minishell: %s: No such file or directory\n",
				cmd->infile);
			return (0);
		}
		close(fd);
	}
	return (1);
}

int	open_redirs(t_cmd *cmd)
{
	if (!validate_input_files(cmd))
		return (0);
	if (!open_infile(cmd))
		return (0);
	if (!open_outfile(cmd))
		return (0);
	return (1);
}
