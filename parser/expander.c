/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:37 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/28 12:02:03 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(char **envp, const char *input, int *i)
{
	char		var[256];
	int	k;
	int m;

	k = 0;
	m = 0;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		var[k++] = input[(*i)++];
	var[k] = '\0';
	if (!var[0])
		return (ft_strdup(""));
	while (envp[m])
	{
		if (!ft_strncmp(envp[m], var, k) && envp[m][k] == '=')
			return (ft_strdup(envp[m] + k + 1));
		m++;
	}
	return (ft_strdup(""));
}

static int		exit_code_logic(int last_exit_code, char *result, int j)
{
	char	*itoa_str;
	int		i;

	i = 0;
	itoa_str = ft_itoa(last_exit_code);
	if (!itoa_str)
		allocation_error(itoa_str);
	while (itoa_str[i])
		result[j++] = itoa_str[i++];
	free(itoa_str);
	return (j);
}

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	char		*result;
	const char	*val;
	int			i;
	int			j;
	int 		v;

	i = 0;
	j = 0;
	v = 0;
	result = malloc(4096);
	allocation_error(result);
	while (input[i])
	{
		if (input[i] == '$')
		{
			i++;
			if (input[i] == '?')
			{
				j = exit_code_logic(last_exit_code, result, j);
				i++;
			}
			else
			{
				val = get_env_value(envp, input, &i);
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