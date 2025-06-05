/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:17:14 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 20:40:20 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_commands(t_cmd *cmd)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	execute_builtin_in_pipeline(t_cmd *current, t_env **env, int cmd_count)
{
	pid_t	pid;

	if (cmd_count == 1)
		return (exec_builtin(current, env));
	pid = fork();
	if (pid == 0)
		exit(exec_builtin(current, env));
	return (pid);
}

static int	wait_for_children(pid_t *pids, int cmd_count, int last_exit_code)
{
	int	final_exit_code;
	int	status;
	int	i;

	final_exit_code = last_exit_code;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		handle_child_exit(status);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				final_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				final_exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (final_exit_code);
}

int	execute_pipeline(t_cmd *cmd, t_env **env, int last_exit_code)
{
	pid_t	*pids;
	int		cmd_count;
	int		final_exit_code;

	cmd_count = count_commands(cmd);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (1);
	if (execute_commands_loop(cmd, env, pids) == -1)
	{
		free(pids);
		return (1);
	}
	final_exit_code = wait_for_children(pids, cmd_count, last_exit_code);
	free(pids);
	return (final_exit_code);
}
