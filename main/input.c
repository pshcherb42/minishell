/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:42:26 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/03 17:53:50 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_sigquit_flag;

char	*get_valid_input(void)
{
	char	*input;

    while (1)
    {
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
        if (validate_quotes(input))
            return (input);
        
        ft_printf("minishell: syntax error: unclosed quote\n");
        free(input);
	}
}
