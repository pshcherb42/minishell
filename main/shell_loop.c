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

/*int	process_input(char *input, t_env **env, int last_exit_code)
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
		//printf("DEBUG: parse_input devolvio no NULL\n");
		exit_code = execute_cmds(cmds, env, last_exit_code);
	}
	if (cmds)
		cleanup_cmd_heredocs(cmds);
	free_cmd_list(cmds);
	//printf("[DEBUG] Finished processing input\n");
	return (exit_code);
}*/

int	process_input(char *input, t_env **env, int last_exit_code)
{
    t_cmd	*cmds;
    int		exit_code;

    exit_code = last_exit_code;
    if (*input)
        add_history(input);
    cmds = parse_input(input, *env, last_exit_code);
    
    if (!cmds)
    {
        // parse_input falló (error de sintaxis o input vacío)
		//printf("DEBUG: parse_input devolvio NULL");
        return (2); // Exit code 2 para errores de sintaxis
    }
    
    if (cmds->heredoc_interrupted)
    {
        cleanup_cmd_heredocs(cmds);
        free_cmd_list(cmds);
        return (130);
    }
    //printf("DEBUG:parse_input no devolvio NULL\n");
    exit_code = execute_cmds(cmds, env, last_exit_code);
    cleanup_cmd_heredocs(cmds);
    free_cmd_list(cmds);
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
