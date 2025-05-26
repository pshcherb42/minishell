/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:20:49 by akreise           #+#    #+#             */
/*   Updated: 2025/05/26 18:00:39 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	set_signals(void)//настраивает поведение сигналов SIGINT и SIGQUIT на стандартное
{
	signal(SIGINT, SIG_DFL);//нажимаем Ctrl+C — команда прерывается
	signal(SIGQUIT, SIG_DFL);//нажимаем Ctrl+\ — появится "Quit (core dumped)" и процесс завершится
}

static	void	redirect_pipes(t_cmd *cmd, int prev_fd, int pipefd[2])
{
	if (prev_fd != -1)//если это не первая команда
	{
		dup2(prev_fd, STDIN_FILENO);//перенаправляем STDIN на конец предыдущего pipe
		close(prev_fd);//закрываем старый дескриптор
	}
	if (cmd->next)//если это не последняя команда
	{
		close(pipefd[0]);//закрываем неиспользуемый конец pipe (чтение)
		dup2(pipefd[1], STDOUT_FILENO);//перенаправляем STDOUT на начало текущего pipe (запись)
		close(pipefd[1]);//закрываем старый дескриптор
	}
}

static	void	exec_child_cmd(t_cmd *cmd, char ***envp)//отвечает за выполнение команды — встроенной или внешней
{
	char	*path;

	if (is_builtin(cmd->args[0]))//Если команда встроенная (типа echo, cd и др.)
		exit(exec_builtin(cmd, envp));//сразу её выполняем через exec_builtin и завершаем процесс с exit
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')//Если путь к команде начинается с / (абсолютный путь) или . (относительный)
	{
		execve(cmd->args[0], cmd->args, *envp);//заменяет текущий процесс новой программойe
		ft_pstr(2, "minishell: ");
		ft_pstr(2, cmd->args[0]);
		ft_pstr(2, ": no such file or directory\n");
		exit(127);
	}      
	path = get_cmd_path(cmd->args[0], *envp);//если путь не был указан явно, ищет команду во всех директориях из переменной окружения PATH.
	if (!path)
	{
		ft_pstr(2, "minishell; ");
        ft_pstr(2, cmd->args[0]);
		ft_pstr(2, "; command not found\n");
		exit(127);
	}
	execve(path, cmd->args, *envp);
	perror("execve");
	free(path);
	exit(EXIT_FAILURE);
}

//она запускается в дочернем процессе после fork, и готовит всё для исполнения команды
void	run_child(t_cmd *cmd, int prev_fd, int pipefd[2], char ***envp)
{
	set_signals();//в родителе SIGINT (Ctrl+C) был игнорирован, но дочерний должен уметь 
	redirect_pipes(cmd, prev_fd, pipefd);//Настраиваем stdin и stdout через dup2
	if (!open_redirs(cmd))//Открывает файлы из редиректов в команде
		exit(EXIT_FAILURE);
	exec_child_cmd(cmd, envp);//Сам запуск команды
}

//status — это код, полученный от waitpid
void	handle_child_exit( int status)//анализирует почему завершился дочерний процесс
{
	if (WIFSIGNALED(status))//возвращает номер сигнала, вызвавшего завершение
	{
		if (WTERMSIG(status) == SIGINT)//когда нажимаешь Ctrl+C, показывают новую строку ввода
			write(1, "\n", 1);
		if (WTERMSIG(status) == SIGQUIT)//нажимаем Ctrl+
			write(2, "Quit (core dumped)\n", 20);
	}
}
