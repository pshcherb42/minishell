/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/12 14:38:08 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;

	(void)argc;
	(void)argv;

	my_env = dup_env(envp);
	init_shell(my_env);
	run_shell_loop(&my_env);
	free_env(my_env);
	rl_clear_history();
	rl_cleanup_after_signal();
	return (0);
}
