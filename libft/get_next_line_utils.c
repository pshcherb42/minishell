/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:35:15 by pshcherb          #+#    #+#             */
/*   Updated: 2025/02/15 13:21:02 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

char	*gnl_strjoin(const char *s1, const char *s2)
{
	int			stringsize;
	char		*res;
	int			i;
	int			j;

	i = 0;
	stringsize = gnl_strlen(s1) + gnl_strlen(s2);
	res = malloc((stringsize + 1) * sizeof(char));
	if (!res || !s1 || !s2)
		return (NULL);
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[stringsize] = '\0';
	return (res);
}

char	*gnl_strchr(const char *string, int treasure)
{
	char	*str;

	str = (char *)string;
	while (*str != '\0' && *str != treasure)
		str++;
	if (*str == treasure)
		return (str);
	else
		return (NULL);
}

void	gnl_bzero(void *s, size_t n)
{
	char		*str;
	size_t		i;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

void	*ft_gnl_calloc(size_t elementCount, size_t elementSize)
{
	char	*res;

	res = malloc(elementCount * elementSize);
	if (!res)
		return (NULL);
	gnl_bzero(res, elementCount * elementSize);
	return (res);
}

size_t	gnl_strlen(const char *string)
{
	size_t	i;

	i = 0;
	while (string[i])
		i++;
	return (i);
}
