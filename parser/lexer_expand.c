/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:17:47 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 15:31:18 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_token(t_split_vars *vars, t_env *envp, int l_e_c)
{
	if (!vars->was_single_quoted && ft_strchr(vars->raw_token, '$'))
	{
		vars->expanded = expand_variables(vars->raw_token, envp, l_e_c);
		free(vars->raw_token);
	}
}
