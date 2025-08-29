/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:16:38 by mlemerci          #+#    #+#             */
/*   Updated: 2024/12/05 11:34:21 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t l);
size_t	ft_strlen(const char *s);

char	*ft_itoa(int n);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *str, int c);
char	*ft_strdup(const char *src);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strrchr(const char *str, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);

int		ft_atoi(const char *str);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_toupper(int c);
int		ft_tolower(int c);

void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t elementCount, size_t elementSize);
void	*ft_memchr(const void *str, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t n);
void	*ft_memset(void *pointer, int c, size_t n);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int nb, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));

#endif