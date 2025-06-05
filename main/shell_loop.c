/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:43:57 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/05 16:49:46 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_input(char *input, t_env **env, int last_exit_code)
{
	t_cmd	*cmds;
	int		exit_code;

	//printf("[DEBUG] Processing input: %s\n", input);
	exit_code = last_exit_code;
	if (*input)
		add_history(input);
	cmds = parse_input(input, *env, last_exit_code);
	if (cmds)
	{
		//printf("[DEBUG] Commands parsed successfully\n");
		if (cmds->heredoc_interrupted)
		{
			//printf("[DEBUG] Heredoc interrupted\n");
			cleanup_cmd_heredocs(cmds);
			free_cmd_list(cmds);
			return (130);
		}
		exit_code = execute_cmds(cmds, env, last_exit_code);
	}
	if (cmds)
		cleanup_cmd_heredocs(cmds);
	free_cmd_list(cmds);
	//printf("[DEBUG] Finished processing input\n");
	return (exit_code);
}

void	run_shell_loop(t_env **env)
{
	char	*input;
	int		last_exit_code;

	//printf("[DEBUG] Entering shell loop\n");
	last_exit_code = 0;
	while (1)
	{
		input = get_valid_input();
		if (!input)
		{
			//printf("[DEBUG] No input, exiting shell loop\n");
			printf("exit\n");
			break ;
		}
		last_exit_code = process_input(input, env, last_exit_code);
		free(input);
	}
}
