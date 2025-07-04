/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:16 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 21:37:00 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

t_cmd	*parse_single_command(char *input, t_env *envp, int l_e_c)
{
	t_cmd	*cmd;
	char	**tokens;

	cmd = init_and_tokenize(input, envp, l_e_c, &tokens);
	if (!cmd)
		return (NULL);
	fill_cmd_from_tokens(cmd, tokens);
	free_args(tokens);
	if (cmd->args && cmd->args[0] == NULL)
	{
		free_cmds(cmd);
		return (NULL);
	}
	return (cmd);
}

t_cmd	*parse_input(char *input, t_env *env, int last_exit_code)
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
	state.env = env;
	state.last_exit_code = last_exit_code;
	result = build_command_list(&state);
	free(state.segments);
	return (result);
}
