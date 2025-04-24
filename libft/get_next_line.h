/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:34:47 by pshcherb          #+#    #+#             */
/*   Updated: 2025/02/15 13:20:28 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:34:47 by pshcherb          #+#    #+#             */
/*   Updated: 2025/02/15 12:20:15 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include "libft.h"

char	*gnl_strjoin(const char *s1, const char *s2);
char	*gnl_strchr(const char *string, int treasure);
void	gnl_bzero(void *s, size_t n);
void	*ft_gnl_calloc(size_t elementCount, size_t elementSize);
size_t	gnl_strlen(const char *string);
char	*ft_free(char *buf, char *buffer);
char	*ft_next(char *buffer);
char	*ft_line(char *line);
char	*ft_read_file(int fd, char *res);
char	*get_next_line(int fd);

#endif

/*#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include "libft.h"

char	*gnl_strjoin(const char *s1, const char *s2);
char	*gnl_strchr(const char *string, int treasure);
void	gnl_bzero(void *s, size_t n);
void	*ft_gnl_calloc(size_t elementCount, size_t elementSize);
size_t	gnl_strlen(const char *string);
char	*ft_free(char *buf, char *buffer);
char	*ft_next(char *buffer);
char	*ft_line(char *line);
char	*ft_read_file(int fd, char *res);
char	*get_next_line(int fd);

#endif*/