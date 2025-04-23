/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:02 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/16 14:43:08 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0); // borra la linea actual
	rl_on_new_line(); // informa a readline que hay nueva linea
	rl_redisplay();   // vuelve a mostrar el prompt
}

void	handle_sigquit(int sig)
{
	(void)sig;
	// ignoramos Ctrl+\ como bash
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint); // Ctrl+C
	signal(SIGQUIT, SIG_IGN); 	   // Ignoramos Ctrl+'\'
}
