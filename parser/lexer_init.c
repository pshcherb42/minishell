/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:06:34 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/01 17:17:13 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_token(t_split_vars *vars, int start)
{
	vars->len = vars->i - start;
	vars->raw_token = malloc(vars->len + 1);
	if (!vars->raw_token)
		return (0);
	vars->k = 0;
	vars->m = start;
	vars->inside_quotes = 0;
	vars->was_single_quoted = 0;
	vars->q_char = '\0';
	return (1);
}
