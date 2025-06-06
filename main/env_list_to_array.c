/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:54 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/06 19:38:17 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	calculate_entry_length(t_env *tmp)
{
	size_t	len;

	len = strlen(tmp->name) + 1;
	if (tmp->value)
		len += strlen(tmp->value);
	return (len);
}

static void	format_env_entry(char *entry, t_env *tmp, size_t len)
{
	(void)len;
	ft_strcpy(entry, tmp->name);
	if (tmp->value)
	{
		ft_strcat(entry, "=");
		ft_strcat(entry, tmp->value);
	}
}

static int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static void	fill_env_array(char **arr, t_env *env)
{
	t_env	*tmp;
	int		i;
	size_t	len;

	tmp = env;
	i = 0;
	while (tmp)
	{
		len = calculate_entry_length(tmp);
		arr[i] = malloc(len + 1);
		format_env_entry(arr[i], tmp, len);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
}

char	**env_list_to_array(t_env *env)
{
	int		count;
	char	**arr;

	count = count_env_nodes(env);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	fill_env_array(arr, env);
	return (arr);
}
