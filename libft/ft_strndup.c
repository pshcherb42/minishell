/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:16:42 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/30 11:25:10 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

char    *ft_strndup(const char *s, size_t n)
{
    size_t  l;
    char    *d;

    l = ft_strnlen(s, n);
    d = malloc(l + 1);
    if (!d)
        return (NULL);
    ft_memcpy(d, s, l);
    d[l] = 0;
    return (d);
}
