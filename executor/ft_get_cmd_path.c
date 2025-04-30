/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:44 by akreise           #+#    #+#             */
/*   Updated: 2025/04/30 19:22:20 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	char	*find_path(char **envp)//ищем PATH= в окружении
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);//возвращааем место после PATH= - путь
		i++;
	}
	return (NULL);
}

static	char	*search_cmd(char **paths, char *cmd)//ищем команду в PATH
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);//склеиваем команду и папку
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0) //проверили существует и работает ли
			return (full_path);//вернули если нашли верный путь
		free(full_path);//освободили если не тот путь
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (ft_strchr(cmd, '/')) //указали путь к команде (./a.out)
		return (ft_strdup(cmd));//вернули копию команды
	path_env = find_path(envp);//ищм PATH в окружении
	if (!path_env)
		return (NULL);
	paths = split_path(path_env);//делим PATH на части по :
	if (!paths)
		return (NULL);
	full_path = search_cmd(paths, cmd);//ищем команду - ее путь
	free_split(paths);
	return (full_path);
}
