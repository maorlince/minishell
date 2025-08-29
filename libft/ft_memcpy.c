/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 22:30:23 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/28 17:57:34 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*d;
	const char	*s = (const char *)src;

	d = (char *)dst;
	i = 0;
	if (!d && !s)
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

/*
int	main(void)
{
	char src[] = "hello you, what's up";
	char dst[20];
	int n = 5;
	ft_memcpy(dst, src, n);
	dst[n - 1] = '\0';
	printf("%s", dst);
}*/