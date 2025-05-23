/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:17:14 by akreise           #+#    #+#             */
/*   Updated: 2025/05/23 18:20:21 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	create_pipe(t_cmd *cmd, int pipefd[2])//занимается созданием pipe (канала) между процессами
{
	if(cmd->next)
	{//pipefd[0] — дескриптор чтения (read end),pipefd[1] — дескриптор записи (write end)
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			return (0);
		}
	}
	return (1);
}

//управляет пайпами в родительском процессе после того, как была создана новая команда и запущен дочерний процесс
static	void	handle_parent_pipe(t_cmd *cmd, int pipefd[2], int *prev_fd)
{
	if (*prev_fd != -1)//Если предыдущий дескриптор был установлен, закрываем его
		close(*prev_fd);//Если не закрывать — утечка дескрипторов
	if (cmd->next)//если есть след команда делаем пайп
	{
		close(pipefd[1]);//write end — родителю он не нужен, его закрываем
		*prev_fd = pipefd[0];//его сохраняем в *prev_fd, чтобы в следующей итерации dup2(prev_fd, STDIN) в дочке подцепил stdin от прошлой команды
	}
}

//создаёшь pipe (у тебя появляется pipefd[0] — читать, pipefd[1] — писать);Данные, которые записываются в pipefd[1], можно прочитать через pipefd[0]
//в дочернем процессе первой команды ты перенаправляешь STDOUT в pipefd[1];
//во второй команде ты читаешь STDIN из pipefd[0]

//перебирает все команды, создаёт пайпы, форкает процессы, запускает дочерние процессы и следит за их завершением
static	int	execute_loop(t_cmd *cmd, char ***envp, int prev_fd, int last_exit_code)//цикл по всем командам
{
	int		pipefd[2];//массив под пайп: pipefd[0] — чтение, pipefd[1] — запись
	pid_t	pid;//ID процесса, получаемый от fork()
	int		status;//код завершения процесса
	int		current_exit_code;

	current_exit_code = last_exit_code;
	while (cmd)//пока у нас есть команды
	{
		if (is_builtin(cmd->args[0]) && is_parent_builtin(cmd->args[0]))//export, cd, unset должны выполняться в родителе
			return (exec_builtin(cmd, envp));//т.к. меняют среду оболочки, исполняем и выходим
		if (!create_pipe(cmd, pipefd))//Создание пайпа, если это не последняя команда
			return (1);
		signal(SIGINT, SIG_IGN);//Игнорируем SIGINT в родителе, родитель не должен умирать от Ctrl+C — только дети
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		if (pid == 0)//в дочернем процессе
			run_child(cmd, prev_fd, pipefd, envp);//там настраивается ввод-вывод, редиректы и вызывается execve или builtin.
		handle_parent_pipe(cmd, pipefd, &prev_fd);//Закрываем старый prev_fd и сохраняем pipefd[0] — для следующей команды
		waitpid(pid, &status, 0);//ждем завершения чаилда
		handle_child_exit(status);//обработка выхода
		if (WIFEXITED(status))
            current_exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            current_exit_code = 128 + WTERMSIG(status);
        else
            current_exit_code = 1;
		signal(SIGINT, handle_sigint);//снова разрешаем ловить Ctrl+C (прерывание), теперь обрабатываем через свою handle_sigint
		signal(SIGQUIT, SIG_IGN);//игнорируем Ctrl+, чтобы minishell не завершился и не скинул core
		cmd = cmd->next;
	}
	return (current_exit_code);
}

//акрываем предыдущий если он был создан, если это не первая команда, 
//если есть далььше команды мы закрываем запись, и сохраняем в перд. 
//для следующей команды чтобы мы прочитали вызод с прошлой команды

int	execute_cmds(t_cmd *cmd, char ***envp, int last_exit_code)//запускает команды и проверки
{
	int prev_fd;//файловый дескриптор предыдущей команды
	int status;//статус завершения последнего дочернего процесса

	prev_fd = -1;//изначально предыдущий дескриптор не существует
	if (!cmd || !cmd->args || !cmd->args[0])//если cmd = NULL или команда пуста
		return (1);
	status = execute_loop(cmd, envp, prev_fd, last_exit_code);//запускаем цикл по командам
	return (status);//безопасный способ узнать, с каким кодом завершилась последняя команда
}

//сли дочерний процесс сделал exit(0), то WEXITSTATUS(status) даст 0;
//если был exit(42), то вернётся 42;
//если процесс был убит сигналом, это не считается "нормальным завершением", и WEXITSTATUS(status) выдаст мусор
