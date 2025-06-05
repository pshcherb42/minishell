/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:17:14 by akreise           #+#    #+#             */
/*   Updated: 2025/06/05 17:56:23 by pshcherb         ###   ########.fr       */
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

static pid_t	execute_single_cmd(t_cmd *cmd, t_env **env, int *prev_fd)
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
	
	/* В родительском процессе управляем пайпами */
	handle_parent_pipe(cmd, pipefd, prev_fd);
	
	return (pid);  /* Возвращаем PID, НЕ ждем завершения */
}

/* НОВАЯ ФУНКЦИЯ: запуск всех команд параллельно */
static int	execute_pipeline(t_cmd *cmd, t_env **env, int last_exit_code)
{
	pid_t	*pids;
	int		cmd_count;
	int		prev_fd;
	int		i;
	t_cmd	*current;
	int		final_exit_code;

	/* Считаем команды */
	cmd_count = 0;
	current = cmd;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}
	
	/* Массив для PID */
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (1);
	
	/* ЗАПУСКАЕМ ВСЕ КОМАНДЫ ПАРАЛЛЕЛЬНО */
	prev_fd = -1;
	current = cmd;
	i = 0;
	while (current)
	{
		/* Проверяем parent builtin команды */
		if (is_builtin(current->args[0]) && is_parent_builtin(current->args[0]))
		{
			/* Parent builtin должен выполняться в родительском процессе */
			if (cmd_count == 1)  /* Одиночная команда */
			{
				free(pids);
				return (exec_builtin(current, env));
			}
			else
			{
				/* Parent builtin в пайпе - форкаем */
				pids[i] = fork();
				if (pids[i] == 0)
				{
					/* В child выполняем как обычную команду */
					exit(exec_builtin(current, env));
				}
			}
		}
		else
		{
			/* Обычная команда */
			pids[i] = execute_single_cmd(current, env, &prev_fd);
			if (pids[i] == -1)
			{
				free(pids);
				return (1);
			}
		}
		
		current = current->next;
		i++;
	}
	
	/* Закрываем последний prev_fd в родителе */
	if (prev_fd != -1)
		close(prev_fd);
	
	/* ЖДЕМ ВСЕ ПРОЦЕССЫ */
	final_exit_code = last_exit_code;
	i = 0;
	while (i < cmd_count)
	{
		int status;
		waitpid(pids[i], &status, 0);
		handle_child_exit(status);
		
		/* Последняя команда определяет exit code */
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
	
	free(pids);
	return (final_exit_code);
}

/* УПРОЩЕННАЯ ФУНКЦИЯ execute_loop */
static int	execute_loop(t_cmd *cmd, t_env **env, int prev_fd, int last_exit_code)
{
	/* Проверяем exit команду */
	if (!ft_strcmp(cmd->args[0], "exit"))
	{
		if (!cmd->next && prev_fd == -1)
			return (ft_exit(cmd->args));
	}
	
	/* Одиночная parent builtin команда */
	if (!cmd->next && prev_fd == -1 && 
		is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))
		return (exec_builtin(cmd, env));
	
	/* Запускаем пайплайн */
	return (execute_pipeline(cmd, env, last_exit_code));
}

/*
static int	execute_single_cmd(t_cmd *cmd, t_env **env, int *prev_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	//printf("[DEBUG] Executing single command: %s\n", cmd->args[0]);
	if (cmd->heredoc && !cmd->heredoc_file)
	{
		//printf("[DEBUG] Heredoc file missing\n");
		return (130);
	}
	if (!create_pipe(cmd, pipefd))
	{
		//printf("[DEBUG] Failed to create pipe\n");
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("[DEBUG] Fork failed");
		return (perror("fork"), 1);
	}
	if (pid == 0)
		run_child(cmd, *prev_fd, pipefd, env);
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

static int	execute_loop(t_cmd *cmd, t_env **env, int prev_fd,
		int last_exit_code)
{
	int	current_exit_code;

	current_exit_code = last_exit_code;
	while (cmd)
	{
		if (!ft_strcmp(cmd->args[0], "exit"))
		{
			if (!cmd->next && prev_fd == -1)
				return (ft_exit(cmd->args));
		}
		if (is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))
			return (exec_builtin(cmd, env));
		current_exit_code = execute_single_cmd(cmd, env, &prev_fd);
		cmd = cmd->next;
	}
	return (current_exit_code);
}*/

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
