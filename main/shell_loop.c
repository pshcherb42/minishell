/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:43:57 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/05 14:23:21 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_input(char *input, char ***envp, int last_exit_code)
{
	//char	*expanded;
	t_cmd	*cmds;
	int		exit_code;

	exit_code = last_exit_code;
	//expanded = expand_variables(input, *envp, last_exit_code);
	if (*input)
		add_history(input);
	cmds = parse_input(input, *envp, last_exit_code);
	if (cmds)
		exit_code = execute_cmds(cmds, envp);
	free_cmd_list(cmds);
	//free(expanded);
	return (exit_code);
}

void	run_shell_loop(char ***envp)
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
		last_exit_code = process_input(input, envp, last_exit_code);
		free(input);
	}
}
