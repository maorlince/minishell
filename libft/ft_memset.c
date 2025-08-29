/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:44:47 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/27 02:52:33 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *pointer, int c, size_t n)
{
	char	*str0;

	str0 = pointer;
	while (n)
	{
		*str0 = (unsigned char)c;
		str0++;
		n--;
	}
	return (pointer);
}

/*
int	main(void)
{
	char str[10];
	char str0[10];

	memset(str, 'a', 9);

	ft_memset(str0, 'b', 9);

	printf("str : %s\n", str);
	printf("str0: %s\n", str0);

	return (0);
}*/