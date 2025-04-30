/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:16 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 15:26:04 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

static t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		free(cmd);
		return (NULL);
	}
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

static int 		handle_redirection(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
		return (-1);
	if (!ft_strcmp(tokens[i], ">") && tokens[i + 1])
		{
			cmd->outfile = ft_strdup(tokens[++i]);
			cmd->append = 0;
			return (i + 1);
		}
		else if (!ft_strcmp(tokens[i], ">>") && tokens[i + 1])
		{
			cmd->outfile = ft_strdup(tokens[++i]);
			cmd->append = 1;
			return (i + 1);
		}
		else if (!ft_strcmp(tokens[i], "<") && tokens[i + 1])
		{
			cmd->infile = ft_strdup(tokens[++i]);
			cmd->heredoc = 0;
			return (i + 1);
		}
		else if (!ft_strcmp(tokens[i], "<<") && tokens[i + 1])
		{	
			cmd->infile = ft_strdup(tokens[++i]);
			cmd->heredoc = 1;
			return (i + 1);
		}
		return (i);
}

static t_cmd 	*init_and_tokenize(char *input, char **envp, int last_exit_code, char ***tokens)
{
	t_cmd *cmd = init_cmd();
	
	if (!cmd)
		return NULL;
	*tokens = split_args(input, envp, last_exit_code);
	if (!*tokens)
	{
		free(cmd->args);
		free(cmd);
		return NULL;
	}
	return cmd;
}

static void 	fill_cmd_from_tokens(t_cmd *cmd, char **tokens)
{
	int 	i;
	int 	j;
	int 	res;

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
			cmd->args[j++] = ft_strdup(tokens[i]);
		i++;
		if (res > i)
			i = res;
	}
	cmd->args[j] = NULL;
}

static void free_token_array(char **tokens)
{
	int 	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

t_cmd	*parse_single_command(char *input, char **envp, int last_exit_code)
{
	t_cmd	*cmd;
	char	**tokens;

	cmd = init_and_tokenize(input, envp, last_exit_code, &tokens);
	if (!cmd)
		return NULL;
	fill_cmd_from_tokens(cmd, tokens);
	free_token_array(tokens);
	return (cmd);
}

static void 	handle_quote(char c, char *quote, int *in_quote)
{
	if ((c == '\'') || c == '"')
	{
		if (!*in_quote)
		{
			*quote = c;
			*in_quote = 1;
		}
		else if (*quote == c)
		{
			*in_quote = 0;
			*quote = '\0';
		}
	}
}

static char	**split_by_pipe(char *input)
{
	t_pipe_state	state;

	state.result = malloc(sizeof(char *) * 100);
	state.quote = '\0';
	state.i = 0;
	state.j = 0;
	state.start = 0;
	state.in_quote = 0;
	while (input[state.i])
	{
		handle_quote(input[state.i], &state.quote, &state.in_quote);
		if (input[state.i] == '|' && !state.in_quote)
		{
			state.result[state.j++] = ft_strndup(input + state.start, state.i - state.start);
			state.start = state.i + 1;
		}
		state.i++;
	}
	state.result[state.j++] = ft_strndup(input + state.start, state.i - state.start);
	state.result[state.j] = NULL;
	return (state.result);
}

static int	 is_empty_or_spaces(const char *str)
{
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return 0;
		str++;
	}
	return (1);
}	

t_cmd	*parse_input(char *input, char **envp, int last_exit_code)
{
	char	**segments;
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new_cmd;
	int 	i;

	segments = split_by_pipe(input);
	head = NULL;
	tail = NULL;
	i = 0;
	while (segments[i])
	{
		if (is_empty_or_spaces(segments[i]))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
			free(segments[i]);
			while (segments[++i])
				free(segments[i]);
			free(segments);
			return NULL;
		}
		new_cmd = parse_single_command(segments[i], envp, last_exit_code);
		if (!new_cmd)
		{
			free(segments[i]);
			i++;
			continue ;
		}
		else
		{
			if (!head)
				head = new_cmd;
			else
				tail->next = new_cmd;
			tail = new_cmd;
		}
		free(segments[i]);
		i++;
	}	
		free(segments);
		return (head);
}
