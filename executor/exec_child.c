/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:20:49 by akreise           #+#    #+#             */
/*   Updated: 2025/06/03 15:19:19 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	set_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static	void	redirect_pipes(t_cmd *cmd, int prev_fd, int pipefd[2])
{
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
}

static	void	exec_child_cmd(t_cmd *cmd, t_env **env)
{
	char	*path;
	char	**envp_arr;

	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd, env));
	envp_arr = env_list_to_array(*env);
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

void	run_child(t_cmd *cmd, int prev_fd, int pipefd[2], t_env **env)
{
	set_signals();
	redirect_pipes(cmd, prev_fd, pipefd);
	if (!open_redirs(cmd))
		exit(EXIT_FAILURE);
	exec_child_cmd(cmd, env);
}

void	handle_child_exit( int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 20);
	}
}
