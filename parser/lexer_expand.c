/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:17:47 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 23:28:03 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*void	expand_token(t_split_vars *vars, t_env *envp, int l_e_c)
{
	printf("DEBUG: Entering expand_token\n");
    printf("DEBUG: raw_token before expansion: %s\n", vars->raw_token);
	if (!vars->was_single_quoted && ft_strchr(vars->raw_token, '$'))
	{
		printf("DEBUG: Token contains '$' and is not single-quoted\n");
		vars->expanded = expand_variables(vars->raw_token, envp, l_e_c);
		printf("DEBUG: raw_token after expansion: %s\n", vars->expanded);
		free(vars->raw_token);
	}
	else
    {
        printf("DEBUG: Token is single-quoted or does not contain '$'\n");
    }
}*/

//#include "../minishell.h"

/*void	expand_token(t_split_vars *vars, t_env *envp, int l_e_c)
{
    printf("DEBUG: Entering expand_token\n");
    printf("DEBUG: raw_token before expansion: %s\n", vars->raw_token);

    // Handle single-quoted tokens (no expansion)
    if (vars->was_single_quoted)
    {
        printf("DEBUG: Token is single-quoted, skipping expansion\n");
        vars->expanded = ft_strdup(vars->raw_token); // Copy as-is
        return;
    }

    // Handle tokens with '$'
    if (ft_strchr(vars->raw_token, '$'))
    {
        printf("DEBUG: Token contains '$', processing expansion\n");

        // Handle special cases like $?, $@, $1, etc.
        if (ft_strcmp(vars->raw_token, "$?") == 0)
        {
            printf("DEBUG: Expanding special variable $?\n");
            vars->expanded = ft_itoa(l_e_c); // Expand to last exit code
        }
        else if (ft_strcmp(vars->raw_token, "$@") == 0)
        {
            printf("DEBUG: Expanding special variable $@\n");
            vars->expanded = ft_strdup(""); // Handle $@ (no positional arguments)
        }
        else if (vars->raw_token[1] && ft_isdigit(vars->raw_token[1]))
        {
            printf("DEBUG: Expanding positional parameter $%c\n", vars->raw_token[1]);
            vars->expanded = ft_strdup(""); // Handle positional parameters (e.g., $1)
        }
        else
        {
            // General variable expansion
            vars->expanded = expand_variables(vars->raw_token, envp, l_e_c);
            if (!vars->expanded)
            {
                printf("DEBUG: Variable not found, replacing with empty string\n");
                vars->expanded = ft_strdup(""); // Replace undefined variables with empty string
            }
        }
        free(vars->raw_token);
    }
    else
    {
        // No expansion needed
        printf("DEBUG: Token does not contain '$', copying as-is\n");
        vars->expanded = ft_strdup(vars->raw_token);
    }
    printf("DEBUG: raw_token after expansion: %s\n", vars->expanded);
}*/

void	expand_tokens(char **tokens, t_env *envp, int l_e_c)
{
    int		i;
    char	*expanded;

    printf("DEBUG: Entering expand_tokens\n");
    for (i = 0; tokens[i]; i++)
    {
        printf("DEBUG: Token before expansion: %s\n", tokens[i]);
        // Skip single-quoted tokens
        if (tokens[i][0] == '\'' && tokens[i][ft_strlen(tokens[i]) - 1] == '\'')
        {
            printf("DEBUG: Token is single-quoted, skipping expansion\n");
            continue;
        }
        // Expand variables in the token
        expanded = expand_variables(tokens[i], envp, l_e_c);
        if (expanded)
        {
            free(tokens[i]);
            tokens[i] = expanded;
        }
        printf("DEBUG: Token after expansion: %s\n", tokens[i]);
    }
}
