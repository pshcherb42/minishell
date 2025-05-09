/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/05/03 18:14:49 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cd(char **args, char **envp) // changes current directory
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
// Bonus: actualizar PWD en entorno, luego lo haremos