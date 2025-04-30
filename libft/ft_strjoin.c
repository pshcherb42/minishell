/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 22:35:51 by pshcherb          #+#    #+#             */
/*   Updated: 2024/09/28 22:49:17 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static size_t	ft_str_copy(char *dest, const char *src, size_t start)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[start + i] = src[i];
		i++;
	}
	return (start + i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*s3;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	s3 = (char *)malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!s3)
		return (NULL);
	ft_str_copy(s3, s1, 0);
	ft_str_copy(s3, s2, len_s1);
	s3[len_s1 + len_s2] = '\0';
	return (s3);
}
