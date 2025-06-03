/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:16 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/03 18:12:43 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

t_cmd	*parse_single_command(char *input, char **envp, int l_e_c)
{
	t_cmd	*cmd;
	char	**tokens;

	tokens = tokenize(input);
	if (!tokens)
		return (NULL);
	cmd = init_and_tokenize(input, envp, l_e_c, &tokens);
	free_args(tokens);
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		if (cmd)
			free_cmds(cmd);
		return (NULL);
	}
	return (cmd);
}

t_cmd	*parse_input(char *input, char **envp, int last_exit_code)
{
	t_parse_state	state;
	t_cmd			*result;

	if (is_empty_or_spaces(input))
		return (NULL);
	state.segments = split_by_pipe(input);
	if (!state.segments)
		return (NULL);
	state.head = NULL;
	state.tail = NULL;
	state.i = 0;
	state.envp = envp;
	state.last_exit_code = last_exit_code;
	result = build_command_list(&state);
	free(state.segments);
	return (result);
}
