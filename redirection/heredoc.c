/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:42:32 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/30 21:24:26 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_here_father(pid_t pid, int *status, char **temp_file)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, status, 0);
	signal(SIGINT, handle_sigint);
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
	{
		write(1, "^C\n", 3);
		if (*temp_file)
		{
			cleanup_temp_file(*temp_file);
			*temp_file = NULL;
		}
		return (-1);
	}
	if (WIFEXITED(*status) && WEXITSTATUS(*status) != 0)
	{
		if (*temp_file)
		{
			cleanup_temp_file(*temp_file);
			*temp_file = NULL;
		}
		return (-1);
	}
	return (0);
}

void	handle_here_child(const char *delimiter, const char *filename)
{
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (handle_here_doc(delimiter, filename) == -1)
		exit(130);
	exit(0);
}

static int	read_heredoc_lines(const char *delimiter, const char *filename)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_sigquit_flag)
		{
			if (line)
				free(line);
			return (-1);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!write_to_temp_file(filename, line))
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

int	handle_heredoc(const char *delimiter, const char *filename)
{
	return (read_heredoc_lines(delimiter, filename));
}

int	handle_here_doc(const char *delimiter, const char *filename)
{
	return (read_heredoc_lines(delimiter, filename));
}
