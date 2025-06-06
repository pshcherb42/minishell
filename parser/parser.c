/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:16 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 22:45:33 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

t_cmd	*parse_single_command(char *input, t_env *envp, int l_e_c)
{
	t_cmd	*cmd;
	char	**tokens;
	//char	**env_array;

	//env_array = env_list_to_array(envp);
	cmd = init_and_tokenize(input, envp, l_e_c, &tokens);
	if (!cmd)
		return (NULL);
	printf("DEBUG: fill cmd from tokens\n");
	fill_cmd_from_tokens(cmd, tokens);
	free_args(tokens);
	//free_env(env_array);
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

	printf("[DEBUG] Parsing input: %s\n", input);
	if (is_empty_or_spaces(input))
	{
		printf("[DEBUG] Input is empty or spaces\n");
		return (NULL);
	}
	state.segments = split_by_pipe(input);
	if (!state.segments)
	{
		printf("[DEBUG] Failed to split input by pipe\n");
		return (NULL);
	}
	state.head = NULL;
	state.tail = NULL;
	state.i = 0;
	state.env = env;
	state.last_exit_code = last_exit_code;
	result = build_command_list(&state);
	free(state.segments);
	printf("[DEBUG] Command list built\n");
	return (result);
}
