/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/22 15:56:19 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(void)
{
	setup_signals();
}

static char	**dup_env(char **envp)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = strdup(envp[i]);
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

int	process_input(char *input, char ***envp, int last_exit_code)
{
	char		*expanded;
	t_cmd		*cmds;

	expanded = expand_variables(input, *envp, last_exit_code);
	cmds = parse_input(input, *envp, last_exit_code);
	for (int i = 0; cmds->args[i]; i++)
    	printf("arg[%d] = [%s]\n", i, cmds->args[i]);
	if (cmds)
		last_exit_code = execute_cmds(cmds, envp);
	free_cmds(cmds);
	free(expanded);
	return (last_exit_code);
}

void	shell_loop(char ***envp)
{
	char	*input; // el input lo que escribimos
	int		last_exit_code; // exit_code

	last_exit_code = 0;
	while (1) // loop infinito
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ; // salimos del loop si no hay input
		}
		if (!validate_quotes(input))
		{
			fprintf(stderr, "minishell: syntax error: unclosed quote\n");
			free(input);
			continue ;
		}
		if (*input)
			add_history(input);
		process_input(input, envp, last_exit_code);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env; // mis envpis

	(void)argc;
	(void)argv;
	init_shell(); // iniciamos señales ctrl+c ctrl+
	my_env = dup_env(envp); // cambiamos los envipis
	shell_loop(&my_env); // loop principal
	free_env(my_env); // liberamos mis envipis
	return (0);
}
