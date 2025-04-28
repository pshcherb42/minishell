/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:47:09 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/28 10:48:00 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void 	allocation_error(char *str)
{
    if (!str)
    {
		ft_printf("Memory allocation failed.\n");
		exit(1);
	}
}