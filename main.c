/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/17 16:39:03 by pshcherb         ###   ########.fr       */
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

void	process_input(char *input, char ***envp)
{
	char		*expanded;
	t_cmd		*cmds;
	static int	last_exit_code;

	last_exit_code = 0;
	expanded = expand_variables(input, *envp, last_exit_code);
	cmds = parse_input(input, *envp, last_exit_code);
	if (cmds)
		last_exit_code = execute_cmds(cmds, envp);
	free_cmds(cmds);
	free(expanded);
}

void	shell_loop(char ***envp)
{
	char	*input;
	//char	*expanded;
	//t_cmd	*cmds;
	//int		last_exit_code;

	//last_exit_code = 0;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (!validate_quotes(input))
		{
			fprintf(stderr, "minishell: syntax error: unclosed quote\n");
			free(input);
			continue ;
		}
		if (*input)
			add_history(input);
		process_input(input, envp);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;

	(void)argc;
	(void)argv;
	init_shell();
	my_env = dup_env(envp);
	shell_loop(&my_env);
	free_env(my_env);
	return (0);
}
