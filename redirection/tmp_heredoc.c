/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 18:31:02 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/30 21:24:35 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_temp_file(void)
{
	static int	counter;
	char		*temp_name;
	char		*num_str;
	int			fd;

	counter = 0;
	num_str = ft_itoa(getpid() + counter++);
	temp_name = ft_strjoin("/tmp/minishell_heredoc_", num_str);
	free(num_str);
	fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(temp_name);
		return (NULL);
	}
	close(fd);
	return (temp_name);
}

void	cleanup_temp_file(char *filename)
{
	if (!filename)
		return ;
	unlink(filename);
	free(filename);
}

int	write_to_temp_file(const char *filename, const char *line)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_APPEND);
	if (fd < 0)
		return (0);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	close(fd);
	return (1);
}

int	handle_here_fork(const char *delimiter, char **temp_file)
{
	pid_t	pid;
	int		status;
	char	*filename;

	filename = create_temp_file();
	if (!filename)
	{
		perror("Error creating temp file");
		return (-1);
	}
	*temp_file = filename;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		cleanup_temp_file(filename);
		*temp_file = NULL;
		return (-1);
	}
	else if (pid == 0)
		handle_here_child(delimiter, filename);
	else
		return (handle_here_father(pid, &status, temp_file));
	return (0);
}
