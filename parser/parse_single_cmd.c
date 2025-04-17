/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_single_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:55:05 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/17 14:43:46 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * 100);
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
	return (cmd);
}

static void	fill_redirection(t_cmd *cmd, char **tokens, int *i)
{
	if (!strcmp(tokens[*i], ">") && tokens[*i + 1])
	{
		cmd->outfile = strdup(tokens[++(*i)]);
		cmd->append = 0;
	}
	else if (!strcmp(tokens[*i], ">>") && tokens[*i + 1])
	{
		cmd->outfile = strdup(tokens[++(*i)]);
		cmd->append = 1;
	}
	else if (!strcmp(tokens[*i], "<") && tokens[*i + 1])
		cmd->infile = strdup(tokens[++(*i)]);
	else if (!strcmp(tokens[*i], "<<") && tokens[*i + 1])
	{
		cmd->infile = strdup(tokens[++(*i)]);
		cmd->heredoc = 1;
	}
}

static void	fill_arguments(t_cmd *cmd, char *token, int *j)
{
	cmd->args[(*j)++] = strdup(token);
}

static void	process_tokens(t_cmd *cmd, char **tokens)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tokens[i])
	{
		if (!strcmp(tokens[i], ">") || !strcmp(tokens[i], "<")
			|| !strcmp(tokens[i], ">>") || !strcmp(tokens[i], "<<"))
			fill_redirection(cmd, tokens, &i);
		else
			fill_arguments(cmd, tokens[i], &j);
		i++;
	}
	cmd->args[j] = NULL;
}

t_cmd	*parse_single_command(char *input, char **envp, int last_exit_code)
{
	t_cmd	*cmd;
	char	**tokens;
	int		k;

	cmd = init_cmd();
	tokens = split_args(input, envp, last_exit_code);
	if (!cmd || !tokens)
		return (NULL);
	process_tokens(cmd, tokens);
	k = 0;
	while (tokens[k])
		free(tokens[k++]);
	free(tokens);
	return (cmd);
}
