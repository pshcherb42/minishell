/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:37 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/24 14:32:07 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(const char *var, char **envp)
{
	int	i;
	int	len;

	i = 0;
	if (!var || !var[0])
		return (ft_strdup(""));
	len = ft_strlen(var);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, len) && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	char		*result;
	char		var[256];
	const char	*val;
	char		*itoa_str;
	int			i;
	int			j;
	int			k;
	int 		t;
	int 		v;

	i = 0;
	j = 0;
	k = 0;
	t = 0;
	v = 0;
	result = malloc(4096);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == '$')
		{
			result[j++] = '$';
			i += 2;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				result[j++] = input[i++];
		}
		else if (input[i] == '$')
		{
			i++;
			if (input[i] == '?')
			{
				itoa_str = ft_itoa(last_exit_code);
				t = 0;
				while (itoa_str[t])
					result[j++] = itoa_str[t++];
				free(itoa_str);
				i++;
			}
			else
			{
				k = 0;
				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
					var[k++] = input[i++];
				var[k] = '\0';
				val = get_env_value(var, envp);
				v = 0;
				while (val[v])
					result[j++] = val[v++];
				free((char *)val);
			}
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}
