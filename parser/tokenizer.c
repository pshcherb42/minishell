/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:05:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/03 17:19:34 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_and_tokenize(char *in, char **env, int lec, char ***tkn)
{
	t_cmd	*cmd;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	*tkn = split_args(in, env, lec);
	if (!*tkn)
	{
		free_cmds(cmd);
		return (NULL);
	}
	return (cmd);
}

static int	add_arg(t_cmd *cmd, char *token, int *j)
{
	if (*j >= cmd->capacity - 1)
	{
		cmd->args = grow_args_array(cmd->args, *j, &cmd->capacity);
		if (!cmd->args)
			return (0);
	}
	cmd->args[*j] = ft_strdup(token);
	if (!cmd->args[*j])
		return (0);
	(*j)++;
	return (1);
}

void	fill_cmd_from_tokens(t_cmd *cmd, char **tokens)
{
	int	i;
	int	j;
	int	res;

	i = 0;
	j = 0;
	while (tokens[i])
	{
		res = handle_redirection(cmd, tokens, i);
		if (res == -1)
		{
			if (j > 0)
			{
				while (j > 0)
				{
					j--;
					free(cmd->args[j]);
					cmd->args[j] = NULL;
				}
			}
			cmd->args[0] = NULL;
			return ;
		}
		if (res == i)
		{
			if (!add_arg(cmd, tokens[i], &j))
			{
				cmd->args[j] = NULL;
				return ;
			}
		}
		if (res > i)
			i = res;
		else
			i++;
	}
	cmd->args[j] = NULL;
}
