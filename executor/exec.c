/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:17:14 by akreise           #+#    #+#             */
/*   Updated: 2025/05/30 21:26:25 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	create_pipe(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			return (0);
		}
	}
	return (1);
}

static	void	handle_parent_pipe(t_cmd *cmd, int pipefd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

static int	execute_single_cmd(t_cmd *cmd, char ***envp, int *prev_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (cmd->heredoc && cmd->heredoc_fd < 0)
		return (130);
	if (!create_pipe(cmd, pipefd))
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		run_child(cmd, *prev_fd, pipefd, envp);
	handle_parent_pipe(cmd, pipefd, prev_fd);
	waitpid(pid, &status, 0);
	handle_child_exit(status);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (1);
}

static int	execute_loop(t_cmd *cmd, char ***envp, int prev_fd,
		int last_exit_code)
{
	int	current_exit_code;

	current_exit_code = last_exit_code;
	while (cmd)
	{
		if (is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))
			return (exec_builtin(cmd, envp));
		current_exit_code = execute_single_cmd(cmd, envp, &prev_fd);
		cmd = cmd->next;
	}
	return (current_exit_code);
}

int	execute_cmds(t_cmd *cmd, char ***envp, int last_exit_code)
{
	int	prev_fd;
	int	status;

	prev_fd = -1;
	if (!cmd)
		return (1);
	if (cmd->heredoc && cmd->heredoc_fd < 0)
		return (130);
	if (!cmd->args || !cmd->args[0])
		return (1);
	status = execute_loop(cmd, envp, prev_fd, last_exit_code);
	return (status);
}
