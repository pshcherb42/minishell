/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:42:32 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 19:48:56 by akreise          ###   ########.fr       */
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

static void	print_heredoc_warning(const char *delimiter)
{
	ft_printf("minishell: warning: here-document delimited by ");
	ft_printf("end-of-file (wanted `%s')\n", delimiter);
}

static int	handle_heredoc_line(char *line, const char *delimiter,
							const char *filename)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (!write_to_temp_file(filename, line))
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

int	read_heredoc_lines(const char *delimiter, const char *filename)
{
	char	*line;
	int		result;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(delimiter);
			return (0);
		}
		if (g_sigquit_flag)
		{
			if (line)
				free(line);
			return (-1);
		}
		result = handle_heredoc_line(line, delimiter, filename);
		if (result == 1)
			break ;
		if (result == -1)
			return (-1);
	}
	return (0);
}
