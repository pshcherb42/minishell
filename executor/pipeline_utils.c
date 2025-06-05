/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:17:14 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 19:14:50 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static pid_t	execute_single_command(t_cmd *current, t_env **env,
	int *prev_fd, int cmd_count)
{
	if (is_builtin(current->args[0]) && is_parent_builtin(current->args[0]))
		return (execute_builtin_in_pipeline(current, env, cmd_count));
	else
		return (execute_single_cmd(current, env, prev_fd));
}

int	execute_commands_loop(t_cmd *cmd, t_env **env, pid_t *pids)
{
	t_cmd	*current;
	int		prev_fd;
	int		i;
	int		cmd_count;

	prev_fd = -1;
	current = cmd;
	i = 0;
	cmd_count = count_commands(cmd);
	while (current)
	{
		pids[i] = execute_single_command(current, env, &prev_fd, cmd_count);
		if (pids[i] == -1)
			return (-1);
		current = current->next;
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (0);
}
