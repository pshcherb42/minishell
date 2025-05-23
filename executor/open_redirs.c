/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/23 16:09:55 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_infile(t_cmd *cmd)//файл для ввода?-cat < input.txt — здесь input.txt это infile
{
	int	fd;//файловый дескриптор (номер открытого файла)

	if (!cmd->infile)//если файл пустой не сущ
		return (1);
	if (cmd->heredoc)//обрабатывается отдельно
	{
		// el cambio
		if (cmd->heredoc_fd < 0)
		return (0);
		dup2(cmd->heredoc_fd, STDIN_FILENO); // Redirige stdin
		close(cmd->heredoc_fd);
		return (1);
	}
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
