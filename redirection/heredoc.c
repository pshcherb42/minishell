/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:42:32 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/29 18:17:51 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		handle_here_father(pid_t pid, int *p_fd, int *status)
{
	close(p_fd[1]);  // el padre no escribe
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
	{
		close(p_fd[0]);
		return (-1);  // heredoc fue cancelado
	}
	return (p_fd[0]);
}

void	handle_here_child(int *p_fd, const char *delimiter)
{
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(p_fd[0]);  // el hijo no necesita leer
	handle_here_doc(p_fd, delimiter);
	close(p_fd[1]);
}

void	handle_here_doc(int *p_fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(p_fd[1], line, ft_strlen(line));
		write(p_fd[1], "\n", 1);
		free(line);
	}
}

int	handle_here_fork(int *p_fd, const char *delimiter)
{
	pid_t	pid;
	int		status;
	
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(p_fd[0]);
		close(p_fd[1]);
		return (-1);
	}
	else if (pid == 0)
	{
		handle_here_child(p_fd,delimiter);
		exit(0);
	}
	else
	{
		p_fd[0] = handle_here_father(pid, p_fd, &status);
		if (p_fd[0] == -1)
			return (-1);  // heredoc cancelado con Ctrl+C
	}
	return (0);
}

int	handle_heredoc(const char *delimiter)
{
	int		p_fd[2];

	if (pipe(p_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (handle_here_fork(p_fd, delimiter) == -1)
		return (-1);
	close(p_fd[1]);
	return (p_fd[0]);
}
