/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:15:38 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/30 21:30:24 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->capacity = ARGS_INIT_CAPACITY;
	cmd->args = malloc(sizeof(char *) * cmd->capacity);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	cmd->heredoc_file = NULL;
	cmd->heredoc_interrupted = 0;
	return (cmd);
}

char	**grow_args_array(char **old_args, int old_size, int *capacity)
{
	char	**new_args;
	int		i;

	*capacity *= 2;
	new_args = malloc(sizeof(char *) * (*capacity));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new_args[i] = old_args[i];
		i++;
	}
	free(old_args);
	return (new_args);
}

void	cleanup_cmd_heredocs(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->heredoc_file)
		{
			cleanup_temp_file(current->heredoc_file);
			current->heredoc_file = NULL;
		}
		current = current->next;
	}
}
