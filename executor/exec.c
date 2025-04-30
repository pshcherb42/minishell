/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 20:05:03 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_infile(t_cmd *cmd)//файл для ввода?-cat < input.txt — здесь input.txt это infile
{
	int	fd;//файловый дескриптор (номер открытого файла)

	if (!cmd->infile)//если файл пустой не сущ
		return (1);
	if (cmd->heredoc)//обрабатывается отдельно
		return (1);
	fd = open(cmd->infile, O_RDONLY);//открыли только для чтения
	if (fd < 0)//файл открыть не получилось (например, его нет)
	{
		perror(cmd->infile);
		return (0);
	}
	dup2(fd, STDIN_FILENO);//Подменяем стандартный ввод (stdin-клавиатура) на наш файл
	close(fd);//закрываем старый файловый дескриптор (fd)
	return (1);
}

static int	open_outfile(t_cmd *cmd)
{
	int	fd;
	
	if (!cmd->outfile)
		return (1);
	if (cmd->append)//если добавляем в конец <<
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);//апп-добавить вконец, креат создаст файл - 0644 — права на файл: владелец может читать/писать, остальные только читать
	else//если перезаписываем файл >
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);//O_TRUNC — очищаем файл перед записью.
	if (fd < 0)
	{
		perror(cmd->outfile);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);//Подменяем стандартный ввод (stdout-экран) на наш файл
	close(fd);//заменили 0-ввод клавы на номер файла, поэтому этот очистили(остается 0 read(0, buffer, 100))
	return (1);
}

int	open_redirs(t_cmd *cmd)//открывает файлы, которые указаны у команды для редиректа ввода/вывода
{
	if (!open_infile(cmd))//если не удалось открыть файл и пришла ошибка
		return (0);
	if (!open_outfile(cmd))
		return (0);
	return (1);
}

/*int	execute_cmds(t_cmd *cmd, char ***envp)
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
		signal(SIGINT, SIG_IGN);
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
			if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
			{
				execve(cmd->args[0], cmd->args, *envp);
				fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]); // prints reason no such file or directory
				exit(127);
			}
			else
			{
				path = get_cmd_path(cmd->args[0], *envp);
				if (!path)
				{
					fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
					exit(127);
				}
				execve(path, cmd->args, *envp);
				perror("minishell");
				free(path);
				exit(127);
			}
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
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			if (WTERMSIG(status) == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
		}
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		cmd = cmd->next;
	}
	return (WEXITSTATUS(status));
}*/

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static int	setup_child_io(t_cmd *cmd, int prev_fd, int *pipefd)
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
	if (!open_redirs(cmd))
		exit(EXIT_FAILURE);
	return (1);
}

static void	execute_child_command(t_cmd *cmd, char ***envp)
{
	char	*path;

	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd, envp));
	path = get_cmd_path(cmd->args[0], *envp);
	if (!path)
	{
		ft_pstr(2, "minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(path, cmd->args, *envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

static void	run_child_process(t_cmd *cmd, int prev_fd, int *pipefd, char ***envp)
{
	setup_child_signals();
	setup_child_io(cmd, prev_fd, pipefd);
	execute_child_command(cmd, envp);
}

static void	handle_parent_pipe(t_cmd *cmd, int *pipefd, int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

static void	handle_child_exit_status(int status)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(2, "Quit (core dumped)\n", 20);
}

int	execute_cmds(t_cmd *cmd, char ***envp)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;

	prev_fd = -1;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	while (cmd)
	{
		if (is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))
			return (exec_builtin(cmd, envp));
		if (cmd->next && pipe(pipefd) < 0)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		if (pid == 0)
			run_child_process(cmd, prev_fd, pipefd, envp);
		handle_parent_pipe(cmd, pipefd, &prev_fd);
		waitpid(pid, &status, 0);
		handle_child_exit_status(status);
		cmd = cmd->next;
	}
	return (WEXITSTATUS(status));
}
