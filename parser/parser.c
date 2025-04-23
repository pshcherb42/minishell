/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:16 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/14 18:04:27 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

t_cmd	*parse_single_command(char *input, char **envp, int last_exit_code)
{
	t_cmd	*cmd;
	char	**tokens;
	int		i;
	int		j;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	tokens = split_args(input, envp, last_exit_code);
	i = 0;
	j = 0;
	cmd->args = malloc(sizeof(char *) * 100);
	if (!cmd->args)
		return (NULL);
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	while (tokens[i])
	{
		if (!strcmp(tokens[i], ">") && tokens[i + 1])
		{
			cmd->outfile = strdup(tokens[++i]);
			cmd->append = 0;
		}
		else if (!strcmp(tokens[i], ">>") && tokens[i + 1])
		{
			cmd->outfile = strdup(tokens[++i]);
			cmd->append = 1;
		}
		else if (!strcmp(tokens[i], "<") && tokens[i + 1])
			cmd->infile = strdup(tokens[++i]);
		else if (!strcmp(tokens[i], "<<") && tokens[i + 1])
		{
			cmd->infile = strdup(tokens[++i]);
			cmd->heredoc = 1;
		}
		else
			cmd->args[j++] = strdup(tokens[i]);
		i++;
	}
	cmd->args[j] = NULL;
	for (int k = 0; tokens[k]; k++)
		free(tokens[k]);
	free(tokens);
	return (cmd);
}

static char	**split_by_pipe(char *input)
{
	char	**result;
	int		i;
	char	quote;
	int		j;
	int		start;
	int		in_quote;

	result = malloc(sizeof(char *) * 100);
	quote = '\0';
	i = 0;
	j = 0;
	start = 0;
	in_quote = 0;
	while (input[i])
	{
		if ((input[i] == '\'') || input[i] == '"')
		{
			if (!in_quote)
			{
				quote = input[i];
				in_quote = 1;
			}
			else if (quote == input[i])
			{
				in_quote = 0;
				quote = '\0';
			}
		}
		else if (input[i] == '|' && !in_quote)
		{
			result[j++] = strndup(input + start, i - start);
			start = i + 1;
		}
		i++;
	}
	result[j++] = strndup(input + start, i - start);
	result[j] = NULL;
	return (result);
}

t_cmd	*parse_input(char *input, char **envp, int last_exit_code)
{
	char	**segments;
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new_cmd;

	segments = split_by_pipe(input);
	head = NULL;
	tail = NULL;
	for (int i = 0; segments[i]; i++)
	{
		new_cmd = parse_single_command(segments[i], envp, last_exit_code);
		if (!new_cmd)
			continue ;
		if (!head)
			head = new_cmd;
		else
			tail->next = new_cmd;
		tail = new_cmd;
		free(segments[i]);
	}
	free(segments);
	return (head);
}
