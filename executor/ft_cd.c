/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:34:46 by akreise           #+#    #+#             */
/*   Updated: 2025/04/24 15:34:48 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cd(char **args, char **envp) // changes current directory
{
	char	*target;
	int		ret;

	(void)envp;
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