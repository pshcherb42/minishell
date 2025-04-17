/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:37 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/16 16:30:26 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

char	*get_env_value(const char *var, char **envp)
{
	int	i;
	int	len;

	if (!var || !var[0])
		return (strdup(""));
	len = strlen(var);
	for (i = 0; envp[i]; i++)
	{
		if (!strncmp(envp[i], var, len) && envp[i][len] == '=')
			return (strdup(envp[i] + len + 1));
	}
	return (strdup(""));
}

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	char		*result;
	int			i;
	int			j;
	char		var[256];
	int			k;
	const char	*val;

	i = 0;
	j = 0;
	k = 0;
	result = malloc(4096);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == '$') // escapar $ dentro de ""
		{
			result[j++] = '$';
			i += 2;
		}
		else if (input[i] == '$')
		{
			i++;
			if (input[i] == '?')
			{
				j += snprintf(result + j, 4096 - j, "%d", last_exit_code);
				i++;
			}
			else
			{
				k = 0;
				while (input[i] && (isalnum(input[i]) || input[i] == '_'))
					var[k++] = input[i++];
				var[k] = '\0';
				val = get_env_value(var, envp);
				j += snprintf(result + j, 4096 - j, "%s", val);
				free((char *)val);
			}
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}
