/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:43:57 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 21:37:20 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_input(char *input, t_env **env, int last_exit_code)
{
	t_cmd	*cmds;
	int		exit_code;

	exit_code = last_exit_code;
	if (*input)
		add_history(input);
	cmds = parse_input(input, *env, last_exit_code);
	if (!cmds)
		return (2);
	if (cmds->heredoc_interrupted)
	{
		cleanup_cmd_heredocs(cmds);
		free_cmd_list(cmds);
		return (130);
	}
	exit_code = execute_cmds(cmds, env, last_exit_code);
	cleanup_cmd_heredocs(cmds);
	free_cmd_list(cmds);
	return (exit_code);
}

void	run_shell_loop(t_env **env)
{
	char	*input;
	int		last_exit_code;

	last_exit_code = 0;
	while (1)
	{
		input = get_valid_input();
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		last_exit_code = process_input(input, env, last_exit_code);
		free(input);
	}
}
