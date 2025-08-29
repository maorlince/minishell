/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:13:35 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/27 07:24:42 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*tmp;

	i = 0;
	tmp = (unsigned char *)str;
	while (i < n)
	{
		if (tmp[i] == (unsigned char)c)
			return (&tmp[i]);
		i++;
	}
	return (0);
}

/*
 int	main(void)
 {
		int     c;
		c = 'a';
			size_t n = 1;
		const char *str = "salut ca va";
		printf("%p\n", memchr(str, c, n));
 }*/