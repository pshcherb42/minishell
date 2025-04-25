/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:13:33 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/25 15:10:38 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	init_shell(void)
{
	setup_signals();
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_cmd	*cmds;
	int		last_exit_code;
	char	**my_env;
	char	*expanded;

	last_exit_code = 0;
	my_env = dup_env(envp);
	(void)argc;
	(void)argv;
	init_shell();
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
		expanded = expand_variables(input, my_env, last_exit_code);
		if (*input)
			add_history(input);
		cmds = parse_input(input, my_env, last_exit_code);
		if (cmds)
			last_exit_code = execute_cmds(cmds, &my_env);
		free_cmds(cmds);
		free(expanded);
		free(input);
	}
	free_env(my_env);
}
