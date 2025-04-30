/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:21:55 by pshcherb          #+#    #+#             */
/*   Updated: 2024/09/22 14:35:38 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static void	ft_memmove_forward(unsigned char *dst,
				const unsigned char *src,
				size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

static void	ft_memmove_backward(unsigned char *dst,
				const unsigned char *src,
				size_t len)
{
	while (len > 0)
	{
		len--;
		dst[len] = src[len];
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (!dst && !src)
		return (NULL);
	if (dst < src)
		ft_memmove_forward((unsigned char *)dst,
			(const unsigned char *)src,
			len);
	else
		ft_memmove_backward((unsigned char *)dst,
			(const unsigned char *)src,
			len);
	return (dst);
}
