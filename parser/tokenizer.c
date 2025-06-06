/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:05:46 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 21:58:33 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_and_tokenize(char *in, t_env *env, int lec, char ***tkn)
{
	t_cmd	*cmd;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	*tkn = split_args(in, env, lec);
	if (!*tkn)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

static char	*remove_quoted_prefix(char *token)
{
	if (ft_strncmp(token, "QUOTED:", 7) == 0)
		return (ft_strdup(token + 7));
	return (ft_strdup(token));
}

static int	add_arg(t_cmd *cmd, char *token, int *j)
{
	if (*j >= cmd->capacity - 1)
	{
		cmd->args = grow_args_array(cmd->args, *j, &cmd->capacity);
		if (!cmd->args)
			return (0);
	}
	cmd->args[*j] = remove_quoted_prefix(token);
	if (!cmd->args[*j])
		return (0);
	(*j)++;
	return (1);
}

static void	cleanup_args_on_error(t_cmd *cmd, int j)
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
}

static int	process_token(t_cmd *cmd, char **tokens, int *i, int *j)
{
	int	res;

	//printf("DEBUG: process_token\n");
	res = handle_redirection(cmd, tokens, *i);
	if (res == -1)
	{
		cleanup_args_on_error(cmd, *j);
		return (-1);
	}
	//printf("Handle heredoc interruption\n");
	if (res == -2) // Handle heredoc interruption
	{
		//printf("cucufu\n");
		cleanup_args_on_error(cmd, *j);
		cmd->heredoc_interrupted = 1;
		return (-2);
	}
	if (res == *i)
	{
		if (!add_arg(cmd, tokens[*i], j))
		{
			cmd->args[*j] = NULL;
			return (-1);
		}
	}
	if (res > *i)
		*i = res;
	else
		(*i)++;
	return (0);
}

void	fill_cmd_from_tokens(t_cmd *cmd, char **tokens)
{
	int	i;
	int	j;
	int	result;

	i = 0;
	j = 0;
	while (tokens[i])
	{
		result = process_token(cmd, tokens, &i, &j);
		if (result == -1)
			return ;
		//printf("fill cmd from tokens\n");
		if (result == -2)
		{
			//printf("cucufu2\n");
			cmd->heredoc_interrupted = 1;
			return ;
		}
	}
	cmd->args[j] = NULL;
}
