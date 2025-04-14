/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:53 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/14 19:13:36 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*join_path(const char *dir, const char *cmd)
{
	char	*full;
	int		len;

	len = strlen(dir) + strlen(cmd) + 2;
	full = malloc(len);
	if (!full)
		return (NULL);
	snprintf(full, len, "%s/%s", dir, cmd);
	return (full);
}

static char	**split_path(const char *path)
{
	char	**result;
	char	*copy;
	char	*token;
	int		i;

	copy = strdup(path);
	i = 0;
	result = malloc(sizeof(char *) * 100);
	if (!result)
		return (NULL);
	token = strtok(copy, ":");
	while (token)
	{
		result[i++] = strdup(token);
		token = strtok(NULL, ":");
	}
	result[i] = NULL;
	free(copy);
	return (result);
}

static void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = NULL;
	paths = NULL;
	full_path = NULL;
	i = 0;
	if (strchr(cmd, '/'))
		return (strdup(cmd));
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	paths = split_path(path_env);
	for (i = 0; paths[i]; i++)
	{
		full_path = join_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_split(paths);
	return (NULL);
}
