/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:26 by akreise           #+#    #+#             */
/*   Updated: 2025/05/09 17:53:12 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	char	**copy_envp(char **envp, int *len)
{
	char	**copy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	*len = i;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			free_token_array(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static	void	sort_envp(char **env)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static	void	print_line(const char *entry)
{
	char	*eq;
	char	*name;
	char	*value;

	eq = ft_strchr(entry, '=');
	if (eq)
	{
		name = ft_substr(entry, 0, eq - entry);
		value = ft_strdup(eq + 1);
		if (!name || !value)
			return ;
		ft_pstr(1, "declare -x ");
		ft_pstr(1, name);
		ft_pstr(1, "=\"");
		ft_pstr(1, value);
		ft_pstr(1, "\"\n");
		free(name);
		free(value);
	}
	else
	{
		ft_pstr(1, "declare -x ");
		ft_pstr(1, entry);
		ft_pstr(1, "\n");
	}
}

void	print_all(char **envp)
{
	int		len;
	int		i;
	char	**sorted;

	sorted = copy_envp(envp, &len);
	if (!sorted)
		return ;
	sort_envp(sorted);
	i = 0;
	while (sorted[i])
	{
		print_line(sorted[i]);
		i++;
	}
	free(sorted);
}

//вляется ли строка допустимым идентификатором переменной окружения
int	is_valid(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_')) //начинаем с буквы или _
		return (0);
	while (str[i] && str[i] != '=') //Остальные до'=' должны быть буквами, цифрами или подчёркиваниями
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (1); //если есть +=, то всё ок
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1); //все гуд
}