/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:44 by akreise           #+#    #+#             */
/*   Updated: 2025/04/24 17:42:41 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_env_var(char ***envp, int index)
{
	int	k;

	k = index;
	free((*envp)[index]);
	while ((*envp)[k])
	{
		(*envp)[k] = (*envp)[k + 1];
		k++;
	}
}

int	ft_unset(char **args, char ***envp)
{
	int	i;
	int	j;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		j = 0;
		while ((*envp)[j])
		{
			if (!ft_strncmp((*envp)[j], args[i], ft_strlen(args[i])) &&
				(*envp)[j][ft_strlen(args[i])] == '=')
			{
				remove_env_var(envp, j);
				continue ;
			}
			j++;
		}
		i++;
	}
	return (0);
}
