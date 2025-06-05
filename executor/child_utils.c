/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:20:49 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 20:50:38 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_external_cmd(t_cmd *cmd, char **envp_arr)
{
	char	*path;

	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
	{
		execve(cmd->args[0], cmd->args, envp_arr);
		ft_pstr(2, "minishell: ");
		ft_pstr(2, cmd->args[0]);
		ft_pstr(2, ": no such file or directory\n");
		free_env(envp_arr);
		exit(127);
	}
	path = get_cmd_path(cmd->args[0], envp_arr);
	if (!path)
	{
		ft_pstr(2, "minishell: ");
		ft_pstr(2, cmd->args[0]);
		ft_pstr(2, ": command not found\n");
		free_env(envp_arr);
		exit(127);
	}
	execve(path, cmd->args, envp_arr);
	perror("execve");
	free(path);
	free_env(envp_arr);
	exit(EXIT_FAILURE);
}

void	exec_child_cmd(t_cmd *cmd, t_env **env)
{
	char	**envp_arr;

	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd, env));
	envp_arr = env_list_to_array(*env);
	execute_external_cmd(cmd, envp_arr);
}
