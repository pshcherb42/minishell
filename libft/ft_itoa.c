/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:32:23 by pshcherb          #+#    #+#             */
/*   Updated: 2024/09/29 17:12:03 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static int	ft_num_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*s;
	size_t		len;
	long		num;

	len = ft_num_len(n);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	num = n;
	s[len] = '\0';
	if (num < 0)
	{
		s[0] = '-';
		num = -num;
	}
	if (num == 0)
		s[0] = '0';
	while (num > 0)
	{
		s[--len] = (num % 10) + 48;
		num /= 10;
	}
	return (s);
}
