/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/31 21:07:10 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static	void	handle_no_path_env(char *cmd)
{
	ft_pstr(2, "minishell: ");
	ft_pstr(2, cmd);
	ft_pstr(2, ": No such file or directory\n");
	exit(127);
}

static	void	handle_command_not_found(char *cmd)
{
	ft_pstr(2, "minishell: ");
	ft_pstr(2, cmd);
	ft_pstr(2, ": command not found\n");
	exit(127);
}

static	void	exec_absolute_or_relative_path(t_cmd *cmd, char ***envp)
{
	execve(cmd->args[0], cmd->args, *envp);
	ft_pstr(2, "minishell: ");
	ft_pstr(2, cmd->args[0]);
	ft_pstr(2, ": No such file or directory\n");
	exit(127);
}

void	exec_child_cmd(t_cmd *cmd, char ***envp)
{
	char	*path;
	char	*path_env;

	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd, envp));
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		exec_absolute_or_relative_path(cmd, envp);
	path_env = get_path_env(*envp);
	if (!path_env || !*path_env)
		handle_no_path_env(cmd->args[0]);
	path = get_cmd_path(cmd->args[0], *envp);
	if (!path)
		handle_command_not_found(cmd->args[0]);
	execve(path, cmd->args, *envp);
	perror("execve");
	free(path);
	exit(EXIT_FAILURE);
}
