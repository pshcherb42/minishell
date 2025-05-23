/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:59 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/03 17:35:08 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_parent_builtin(const char *cmd) // same as builtin but for pipes
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset"));
}

int	is_builtin(char *cmd) // checks if the given command is builtin
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "export"));
}

int	exec_builtin(t_cmd *cmd, char ***envp) // executes the builtin
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (ft_exit(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (ft_cd(cmd->args, *envp));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (ft_pwd());
	else if (!ft_strcmp(cmd->args[0], "echo"))
		return (ft_echo(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (ft_env(*envp));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (ft_unset(cmd->args, envp));
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (ft_export(cmd->args, envp));
	return (1);
}
