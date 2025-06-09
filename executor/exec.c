/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:17:14 by akreise           #+#    #+#             */
/*   Updated: 2025/06/09 15:02:24 by akreise          ###   ########.fr       */
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

pid_t	execute_single_cmd(t_cmd *cmd, t_env **env, int *prev_fd)
{
	int		pipefd[2];
	pid_t	pid;

	if (cmd->heredoc && !cmd->heredoc_file)
		return (-1);
	if (!create_pipe(cmd, pipefd))
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		run_child(cmd, *prev_fd, pipefd, env);
	handle_parent_pipe(cmd, pipefd, prev_fd);
	return (pid);
}

static int	execute_loop(t_cmd *cmd, t_env **env, int prev_fd,
	int last_exit_code)
{
	if (!ft_strcmp(cmd->args[0], "exit"))
	{
		if (!cmd->next && prev_fd == -1)
			return (ft_exit(cmd->args, last_exit_code));
	}
	if (!cmd->next && prev_fd == -1
		&& is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))
		return (exec_builtin(cmd, env));
	return (execute_pipeline(cmd, env, last_exit_code));
}

int	execute_cmds(t_cmd *cmd, t_env **env, int last_exit_code)
{
	int	prev_fd;
	int	status;

	prev_fd = -1;
	if (!cmd)
		return (1);
	if (cmd->heredoc && !cmd->heredoc_file)
		return (130);
	if (!cmd->args || !cmd->args[0])
		return (1);
	status = execute_loop(cmd, env, prev_fd, last_exit_code);
	return (status);
}
