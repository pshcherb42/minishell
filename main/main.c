/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/05 13:12:07 by pshcherb         ###   ########.fr       */
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
