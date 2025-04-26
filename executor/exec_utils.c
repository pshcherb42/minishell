/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:53 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/26 19:38:37 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_path(const char *dir, const char *cmd)//Создаёт строку "/usr/bin/ls" из "usr/bin" и "ls"
{
	char	*full;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;// считаем длину +1 для '/' и +1 для '\0'
	full = malloc(sizeof(char) * len);
	if (!full)
		return (NULL);
	i = 0;
	while (dir[i])
	{
		full[i] = dir[i];
		i++;
	}
	full[i++] = '/';
	j = 0;
	while (cmd[j])
		full[i++] = cmd[j++];
	full[i] = '\0';
	return (full);
}

static int	find_end(const char *path, int start)//ищем конец сегмента - часть до : 
{
	while (path[start] && path[start] != ':')
		start++;
	return (start);
}

void	free_split(char **arr)//освобождаем массив строк
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static int	update_start(const char *path, int end)//обновляем старт на следующий сегмент
{
	if (path[end] == ':')//если сейчас стоит ':', пропускаем его
		return (end + 1);
	else
		return (end);//иначе остаёмся и старт = /0 и выходим из вайл
}

char	**split_path(const char *path)//делим PATH нв части - разделение по :
{
	char	**result;
	int		start;
	int		end;
	int		i;

	result = malloc(sizeof(char *) * 100);
	if (!result)
		return (NULL);
	start = 0;
	i = 0;
	while (path[start])
	{
		end = find_end(path, start);//находим конец текущего сегмента
		result[i] = ft_substr(path, start, end - start);//вырезаем сегмент и кладем в массив
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
		start = update_start(path, end);//обновляем стартовую позицию
	}
	result[i] = NULL;
	return (result);
}
