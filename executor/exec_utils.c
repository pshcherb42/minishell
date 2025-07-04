/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:53 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/26 20:41:37 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_path(const char *dir, const char *cmd)
{
	char	*full;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
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

static int	find_end(const char *path, int start)
{
	while (path[start] && path[start] != ':')
		start++;
	return (start);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static int	update_start(const char *path, int end)
{
	if (path[end] == ':')
		return (end + 1);
	else
		return (end);
}

char	**split_path(const char *path)
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
		end = find_end(path, start);
		result[i] = ft_substr(path, start, end - start);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
		start = update_start(path, end);
	}
	result[i] = NULL;
	return (result);
}
