/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/15 12:15:44 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_redirs(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		if (cmd->heredoc)
			return (1);
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			return (0);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->outfile)
	{
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
	}
	return (1);
}

int	execute_cmds(t_cmd *cmd, char ***envp)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	char	*path;

	prev_fd = -1;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	while (cmd)
	{
		if (is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))
		{
			return (exec_builtin(cmd, envp));
		}
		if (cmd->next && pipe(pipefd) < 0)
		{
			perror("pipe");
			return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			if (!open_redirs(cmd))
				exit(EXIT_FAILURE);
			if (is_builtin(cmd->args[0]))
				exit(exec_builtin(cmd, envp));
			path = get_cmd_path(cmd->args[0], *envp);
			if (!path)
			{
				fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
				exit(127);
			}
			execve(path, cmd->args, *envp);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
		}
		cmd = cmd->next;
	}
	return (WEXITSTATUS(status));
}
