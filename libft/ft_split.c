/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 23:28:30 by pshcherb          #+#    #+#             */
/*   Updated: 2024/09/29 00:04:46 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static size_t	ft_count_words(char const *s, char c)
{
	size_t	count;
	int		in_substring;

	count = 0;
	in_substring = 0;
	while (*s)
	{
		if (*s != c && in_substring == 0)
		{
			in_substring = 1;
			count++;
		}
		else if (*s == c)
			in_substring = 0;
		s++;
	}
	return (count);
}

static size_t	ft_word_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**ft_allocate_words(char **result,
					char const *s, char c, size_t word_count)
{
	size_t	i;
	size_t	word_len;

	i = 0;
	while (i < word_count)
	{
		while (*s == c)
			s++;
		word_len = ft_word_len(s, c);
		result[i] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (!result[i])
			return (NULL);
		ft_strlcpy(result[i], s, word_len + 1);
		s += word_len;
		i++;
	}
	return (result);
}

static void	ft_free_all(char **result, size_t i)
{
	i = 0;
	while (result[i])
		free(result[i++]);
	free(result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = ft_count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	if (!ft_allocate_words(result, s, c, word_count))
	{
		ft_free_all(result, word_count);
		return (NULL);
	}
	result[word_count] = NULL;
	return (result);
}
