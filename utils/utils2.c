/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:22:21 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/05 12:56:26 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_token_array(cmd->args);
		//free(cmd->cmd); 
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = tmp;
	}
}