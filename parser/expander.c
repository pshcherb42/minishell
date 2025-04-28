/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:37 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/28 10:41:32 by pshcherb         ###   ########.fr       */
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

static int		exit_code_logic(int last_exit_code, char *result, int j)
{
	char	*itoa_str;
	int		i;

	i = 0;
	itoa_str = ft_itoa(last_exit_code);
	if (!itoa_str)
	{
		ft_printf("Memory allocation failed in exit_code_logic.\n");
		exit(1);
	}
	while (itoa_str[i])
		result[j++] = itoa_str[i++];
	free(itoa_str);
	return (j);
}

static char		*copy_name(const char *input, int i, char *var)
{
	int		j;

	j = 0;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		var[j++] = input[i++];
	var[j] = '\0';
	return (i);
}

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	char		*result;
	char		var[256];
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
				i = copy_name(input, i, var);
				val = get_env_value(var, envp);
				v = 0;
				while (val[v])
					result[j++] = val[v++];
				free((char *)val);
			}
			/*{
				k = 0;
				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_')) // el nombre de la varibale puede contener numeros letras o _
					var[k++] = input[i++]; // copiamos el nombre de la varibale 
				var[k] = '\0';
				val = get_env_value(var, envp); // pasamos el nombre de la varibale para expandir
				v = 0;
				while (val[v])
					result[j++] = val[v++];
				free((char *)val);
			}*/
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}