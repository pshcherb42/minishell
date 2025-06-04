/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:36:58 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/04 15:22:36 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_shlvl(char ***envp)
{
	char	*lvl_str;
	int		lvl;
	char	*new_lvl;

	lvl_str = find_env_var(*envp, "SHLVL");
	if (!lvl_str)
		lvl = 1;
	else
		lvl = ft_atoi(lvl_str) + 1;
	if (lvl < 0)
		lvl = 0;
	else if (lvl > 999)
		lvl = 1;
	new_lvl = ft_itoa(lvl);
	if (!new_lvl)
		return ;
	set_env_value(new_lvl, envp);
	free(new_lvl);
}

void	init_shell(char ***envp)
{
	setup_signals();
	update_shlvl(envp);
}
