/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:37 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/25 12:57:03 by pshcherb         ###   ########.fr       */
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

static void 	allocation_error(char *str)
{
    if (!str)
    {
		ft_printf("Memory allocation failed.\n");
		exit(1);
	}
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
	allocation_error(result);
	/*if (!result)
		return (NULL);*/
	while (input[i])
	{
		/*if (input[i] == '\\' || input[i] == ';')
		{
			ft_printf("Unsupported character: '\\', ';'");
			return (NULL);
			result[j++] = '$';
			i += 2;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				result[j++] = input[i++];
		}*/
		if (input[i] == '$')
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

/*static int	copy_exit_code_to_result(char *result, int j, int last_exit_code)
{
	char	*itoa_str;
	int		t;

	t = 0;
	itoa_str = ft_itoa(last_exit_code);
	while (itoa_str[t])
		result[j++] = itoa_str[t++];
	free(itoa_str);
	return (j);
}

static int	copy_env_value_to_result(char *result, int j, const char *var, char **envp)
{
	const char	*val;
	int			v;

	v = 0;
	val = get_env_value(var, envp);
	while (val[v])
		result[j++] = val[v++];
	free((char *)val);
	return (j);
}

static int	handle_dollar_variable(const char *input, char *result, int *i, int j, char **envp, int last_exit_code)
{
	char	var[256];
	int		k = 0;

	if (input[*i] == '?')
	{
		(*i)++;
		return copy_exit_code_to_result(result, j, last_exit_code);
	}
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		var[k++] = input[(*i)++];
	var[k] = '\0';
	return copy_env_value_to_result(result, j, var, envp);
}

static int	handle_escaped_dollar(const char *input, char *result, int *i, int j)
{
	j += (result[j++] = '$', 0);
	*i += 2;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		result[j++] = input[(*i)++];
	return j;
}

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	char	*result;
	int		i = 0, j = 0;

	result = malloc(4096);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == '$')
			j = handle_escaped_dollar(input, result, &i, j);
		else if (input[i] == '$')
		{
			i++;
			j = handle_dollar_variable(input, result, &i, j, envp, last_exit_code);
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}*/
