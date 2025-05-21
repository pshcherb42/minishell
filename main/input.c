/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:42:26 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/20 16:04:44 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t g_sigquit_flag;

char	*get_valid_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (g_sigquit_flag)
	{
		free(input);
		g_sigquit_flag = 0;
		printf("Quit (core dumped)\n");
		exit(131);
	}
	if (!input)
		return (NULL);
	if (!validate_quotes(input))
	{
		ft_printf("minishell: syntax error: unclosed quote\n");
		free(input);
		return (get_valid_input());
	}
	return (input);
}
