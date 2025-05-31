/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/31 19:46:18 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	check_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (i > 1)
		{
			ft_pstr(2, "minishell: cd: too many arguments\n");
			return (1);
		}
		i++;
	}
	return (0);
}

char	*get_current_dir(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_pstr(2, "minishell: cd: cannot get current directory\n");
		return (NULL);
	}
	return (cwd);
}

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

static int	perform_cd_operation(char *target, char *oldpwd,
		char **envp, int is_dash)
{
	char	*newpwd;

	if (try_change_dir(target))
	{
		free(target);
		free(oldpwd);
		return (1);
	}
	if (is_dash)
	{
		newpwd = get_current_dir();
		if (newpwd)
		{
			ft_pstr(1, newpwd);
			ft_pstr(1, "\n");
			free(newpwd);
		}
	}
	update_env_vars(oldpwd, envp);
	free(target);
	free(oldpwd);
	return (0);
}

int	ft_cd(char **args, char **envp)
{
	char	*target;
	char	*oldpwd;
	int		is_dash_arg;

	if (check_args(args))
		return (1);
	is_dash_arg = (args[1] && ft_strcmp(args[1], "-") == 0);
	target = get_target_path(args, envp);
	if (!target)
		return (1);
	oldpwd = get_current_dir();
	if (!oldpwd)
	{
		free(target);
		return (1);
	}
	return (perform_cd_operation(target, oldpwd, envp, is_dash_arg));
}
