/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:42:32 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/16 20:17:22 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_quoted(const char *str)
{
	size_t len = ft_strlen(str);
	return (len >= 2 && (str[0] == '\'' || str[0] == '"')
			&& str[len - 1] == str[0]);
}

static char	*strip_quotes(const char *str)
{
	size_t len = ft_strlen(str);
	if (is_quoted(str))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

void	handle_here_doc(int *p_fd, const char *delimiter, int expand)
{
	char			*line;
	t_split_vars	vars;
	char			*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			expanded = expand_token(vars.expanded);
			write(p_fd[1], expanded, ft_strlen(expanded));
			write(p_fd[1], "\n", 1);
			free(expanded);
		}
		else
		{
			write(p_fd[1], line, ft_strlen(line));
			write(p_fd[1], "\n", 1);
		}
		free(line);
	}
}

int	handle_heredoc(const char *delimiter)
{
	int		p_fd[2];
	char	*clean_delim;
	int		expand;

	if (pipe(p_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	expand = !is_quoted(delimiter);
	clean_delim = strip_quotes(delimiter);
	if (!clean_delim)
		return (-1);
	handle_here_doc(p_fd, delimiter, expand);
	free(clean_delim);
	close(p_fd[1]);
	return (p_fd[0]);
}
