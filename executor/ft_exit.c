/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:33:55 by akreise           #+#    #+#             */
/*   Updated: 2025/05/29 16:29:57 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	is_in_llrange(const char *str)
{
	int		sign;
	int		i;
	char	*max;
	char	*min;

	sign = 1;
	i = 0;
	max = "9223372036854775807";
	min = "9223372036854775808";
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] == '0')
		i++;
	if (ft_strlen(&str[i]) > 19)
		return (0);
	if (ft_strlen(&str[i]) < 19)
		return (1);
	if (sign == 1)
		return (strncmp(&str[i], max, 19) <= 0);
	else
		return (strncmp(&str[i], min, 19) < 0);
}

int	ft_exit_child(char **args)
{
	if (args[1])
	{
		if (!ft_isnumeric(args[1]) || !is_in_llrange(args[1]))
		{
			ft_pstr(2, "minishell: exit: ");
			ft_pstr(2, args[1]);
			ft_pstr(2, ": numeric argument required\n");
			exit(2);
		}
		if (args[2])
		{
			ft_pstr(2, "minishell: exit: too many arguments\n");
			exit(1);
		}
		exit(ft_atoi(args[1]) % 256);
	}
	exit(0);
}

int	ft_exit(char **args)
{
	ft_printf("exit\n");
	if (args[1])
	{
		if (!ft_isnumeric(args[1]) || !is_in_llrange(args[1]))
		{
			ft_pstr(2, "minishell: exit: ");
			ft_pstr(2, args[1]);
			ft_pstr(2, ": numeric argument required\n");
			exit(2);
		}
		if (args[2])
		{
			ft_pstr(2, "minishell: exit: too many arguments\n");
			return (1);
		}
		exit(ft_atoi(args[1]) % 256);
	}
	exit(0);
}
