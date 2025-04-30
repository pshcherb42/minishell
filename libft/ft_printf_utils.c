/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:49:05 by pshcherb          #+#    #+#             */
/*   Updated: 2024/10/08 11:38:36 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (write(1, "(null)", 6));
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putnbr(int n)
{
	int		len;
	char	c;

	len = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		len += write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
		len += ft_putnbr(n / 10);
	c = n % 10 + '0';
	len += write(1, &c, 1);
	return (len);
}

int	ft_puthex(unsigned long long num, char format)
{
	int		count;
	char	*base;

	count = 0;
	if (format == 'x')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (num >= 16)
		count += ft_puthex(num / 16, format);
	count += ft_putchar(base[num % 16]);
	return (count);
}

int	ft_putptr(void *pt)
{
	int					count;
	unsigned long long	ptr;

	ptr = (unsigned long long)pt;
	count = 0;
	if (ptr == 0)
	{
		count += ft_putstr("(nil)");
		return (count);
	}
	count += ft_putstr("0x");
	count += ft_puthex(ptr, 'x');
	return (count);
}

int	ft_putunsigned(unsigned int n)
{
	if (n >= 10)
		return (ft_putunsigned(n / 10)
			+ ft_putchar((n % 10) + '0'));
	else
		return (ft_putchar(n + '0'));
}
