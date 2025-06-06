/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 22:26:29 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env *my_env;

	printf("[DEBUG] Entering main\n");
	(void)argc;
	(void)argv;
	my_env = env_list_from_array(envp);
	printf("[DEBUG] Environment initialized\n");
	init_shell(&my_env);
	printf("[DEBUG] Shell initialized\n");
	run_shell_loop(&my_env);
	printf("[DEBUG] Exiting shell loop\n");
	env_list_free(my_env);
	clear_history();
	rl_cleanup_after_signal();
	printf("[DEBUG] Exiting main\n");
	return (0);
}
