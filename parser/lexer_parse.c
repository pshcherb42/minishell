/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:20:20 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 15:34:17 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_token(t_split_vars *vars, char *input, char **envp, int l_e_c)
{
	int		start;

	start = vars->i;
	vars->len = 0;
	read_token(vars, input);
	if (!init_token(vars, start))
		return (0);
	if (!copy_token(vars, input))
		return (0);
	vars->raw_token[vars->k] = '\0';
	vars->expanded = vars->raw_token;
	expand_token(vars, envp, l_e_c);
	if (vars->expanded == NULL)
		return (0);
	return (1);
}

char	**split_args(char *input, char **envp, int last_exit_code)
{
	char			**args;
	t_split_vars	*vars;

	vars = init_vars();
	if (!vars)
		return (NULL);
	args = malloc(sizeof(char *) * 100);
	if (!args)
		return (NULL);
	while (input[vars->i])
	{
		while (input[vars->i] == ' ' && !vars->in_quote)
			vars->i++;
		if (!input[vars->i])
			break ;
		if (!parse_token(vars, input, envp, last_exit_code))
			return (free_and_return_null(vars, args));
		args[vars->j++] = vars->expanded;
	}
	args[vars->j] = NULL;
	return (args);
}
