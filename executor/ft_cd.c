/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/12 15:02:51 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	check_args(char **args)
{
	if (args[2])
	{
		ft_pstr(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

// Получает текущую директорию (используется для OLDPWD и PWD)
static char	*get_current_dir(void)
{
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);// Просто возвращаем NULL, если не удалось получить директорию
	return (ft_strdup(cwd));// Возвращаем текущую директорию
}

// Пытается сменить директорию 
static int	try_change_dir(char *target)
{
	if (chdir(target) != 0)
	{
		ft_pstr(2, "minishell: cd: ");
		ft_pstr(2, target);
		ft_pstr(2, ": No such file or directory\n");
		return (1);
	}
	return (0);
}

static void	update_env_vars(char *oldpwd, char **envp)
{
	char	*newpwd;

	newpwd = get_current_dir();
	if (!newpwd)
		return ;// Если не удалось получить текущую директорию, не обновляем PWD
	replace_env("OLDPWD", oldpwd, envp);
	replace_env("PWD", newpwd, envp);
	free(newpwd);
}

int	ft_cd(char **args, char **envp)
{
	char	*target;
	char	*oldpwd;

	if (check_args(args))// Проверка на правильность количества аргументов
		return (1);
	target = get_target_path(args);// Получаем путь для смены директории
	if (!target)
		return (1);
	oldpwd = get_current_dir();// Получаем текущую директорию для OLDPWD
	if (!oldpwd)
	{
		free(target);
		return (1);
	}
	if (try_change_dir(target))// Пытаемся сменить директорию
	{
		free(target);
		free(oldpwd);
		return (1);
	}
	update_env_vars(oldpwd, envp);// Обновляем PWD и OLDPWD в окружении
	free(target);
	free(oldpwd);
	return (0);
}
