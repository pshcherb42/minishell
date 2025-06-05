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

void	update_shlvl(t_env **env)
{
	char	*shlvl_value;
	int		lvl;
	char	*new_lvl;

	shlvl_value = find_env_var(*env, "SHLVL");
	if (!shlvl_value)
		lvl = 0;
	else
		lvl = ft_atoi(shlvl_value) + 1;
	if (lvl < 0)
		lvl = 0;
	else if (lvl > 999)
		lvl = 1;
	new_lvl = ft_itoa(lvl);
	if (!new_lvl)
		return ;
	env_list_set(env, "SHLVL", new_lvl);
	free(new_lvl);
}

void	init_shell(t_env **env)
{
	setup_signals();
	update_shlvl(env);
}
