/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/11 19:31:36 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*int	ft_cd(char **args, char **envp) // changes current directory
{
	char	*target;
	int		ret;

	(void)envp;
	if (args[2])
	{
		ft_pstr(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!args[1])
	{
		target = getenv("HOME");
		if (!target)
		{
			ft_pstr(2, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		target = args[1];
	ret = chdir(target);
	if (ret != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
*/

// Создает строку вида VAR=VALUE
static char	*join_env_entry(const char *var_name, const char *value)
{
	char	*entry;
	size_t	var_len;
	size_t	val_len;

	var_len = ft_strlen(var_name);
	val_len = ft_strlen(value);
	entry = malloc(var_len + val_len + 2); // +1 for '=' and +1 for '\0'
	if (!entry)
		return (NULL);
	ft_memcpy(entry, var_name, var_len);
	entry[var_len] = '=';
	ft_memcpy(entry + var_len + 1, value, val_len);
	entry[var_len + val_len + 1] = '\0';
	return (entry);
}

void	replace_env(const char *var_name, const char *value, char **envp)
{
	int		i;
	size_t	len;
	char	*new_entry;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
		{
			free(envp[i]);
			new_entry = join_env_entry(var_name, value);
			if (!new_entry)
				return ;
			envp[i] = new_entry;
			return ;
		}
		i++;
	}
	new_entry = join_env_entry(var_name, value);
	if (!new_entry)
		return ;
	envp[i] = new_entry;
	envp[i + 1] = NULL;
}

static	int	check_args(char **args)
{
	if (args[2])
	{
		ft_pstr(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

// Получает путь для "cd -" из переменной окружения OLDPWD
static	char	*get_oldpwd(void)
{
	char	*oldpwd;

	oldpwd = getenv("OLDPWD"); // Получаем OLDPWD из envp
	if (!oldpwd)
		return (NULL);
	ft_pstr(1, oldpwd);  // Печатаем старую директорию
	ft_pstr(1, "\n");
	return (ft_strdup(oldpwd));  // Возвращаем старую директорию
}

// Получает целевой путь для смены директории (для cd - или из аргументов)
static char	*get_target_path(char **args)
{
	char	*home;
	char	*rest;
	char	*full_path;
	size_t	home_len;
	size_t	rest_len;

	if (args[1] && ft_strcmp(args[1], "-") == 0)
		return (get_oldpwd());
	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
			return (NULL);
		return (ft_strdup(home));
	}
	if (args[1][0] == '~')
	{
		home = getenv("HOME");
		if (!home)
			return (NULL);
		rest = args[1] + 1;
		home_len = ft_strlen(home);
		rest_len = ft_strlen(rest);
		full_path = malloc(home_len + rest_len + 1);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, home, home_len);
		ft_memcpy(full_path + home_len, rest, rest_len);
		full_path[home_len + rest_len] = '\0';
		return (full_path);
	}
	return (ft_strdup(args[1]));
}

// Получает текущую директорию (используется для OLDPWD и PWD)
static char	*get_current_dir(void)
{
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);  // Просто возвращаем NULL, если не удалось получить директорию
	return (ft_strdup(cwd));  // Возвращаем текущую директорию
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

	// Проверка на правильность количества аргументов
	if (check_args(args))
		return (1);
	// Получаем путь для смены директории
	target = get_target_path(args);
	if (!target)
		return (1);
	// Получаем текущую директорию для OLDPWD
	oldpwd = get_current_dir();
	if (!oldpwd)
	{
		free(target);
		return (1);
	}
	// Пытаемся сменить директорию
	if (try_change_dir(target))
	{
		free(target);
		free(oldpwd);
		return (1);
	}
	// Обновляем PWD и OLDPWD в окружении
	update_env_vars(oldpwd, envp);
	free(target);
	free(oldpwd);
	return (0);
}