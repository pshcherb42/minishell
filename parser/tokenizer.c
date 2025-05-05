/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:05:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/02 15:15:15 by pshcherb         ###   ########.fr       */
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
		free(cmd->args);
		free(cmd);
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
	cmd->args[(*j)++] = ft_strdup(token);
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
			cmd->args[0] = NULL;
			return ;
		}
		if (res == i)
		{
			if (!add_arg(cmd, tokens[i], &j))
				return ;
		}
		if (res > i)
			i = res;
		else
			i++;
	}
	cmd->args[j] = NULL;
}

void	free_token_array(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
