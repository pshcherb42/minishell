/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akreise <akreise@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:44 by akreise           #+#    #+#             */
/*   Updated: 2025/05/03 16:22:45 by akreise          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	is_n_flag(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] != '-' || arg[1] != 'n')//если первый не "-" и второй не "n"
		return (0);//ошибка
	while (arg[i])
	{
		if (arg[i] != 'n')// если после -n встречается что то кроме n
			return (0);//ошибка
		i++;
	}
	return (1);
}

int	ft_echo(char **args) // prints arguments, supports -n
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;//по умолчанию печатаем с новой строки
	while (args[i] && is_n_flag(args[i]))//Пока аргумент существует и является флагом -n
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])//Между аргументами ставим пробел, но не после последнего
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
