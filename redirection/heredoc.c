/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:42:32 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/16 19:41:51 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	handle_heredoc(const char *delimiter)
{
	int		p_fd[2];

	if (pipe(p_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	handle_here_doc(p_fd, delimiter);
	close(p_fd[1]);
	return (p_fd[0]);
}
