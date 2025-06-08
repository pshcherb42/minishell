/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/05 19:30:40 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*my_env;

	(void)argc;
	(void)argv;
	my_env = env_list_from_array(envp);
	init_shell(&my_env);
	run_shell_loop(&my_env);
	env_list_free(my_env);
	clear_history();
	rl_cleanup_after_signal();
	return (0);
}
