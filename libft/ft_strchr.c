/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:45:36 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/29 18:54:12 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	int				i;
	unsigned char	chr;

	i = 0;
	chr = (unsigned char)c;
	while (str[i])
	{
		if (str[i] == chr)
			return ((char *)&str[i]);
		i++;
	}
	if (chr == '\0')
		return ((char *)&str[i]);
	return (NULL);
}

/*int	main(void)
 {
	int	c;
	c = 'a';
	const char str[] = "salut";
	printf("%s\n", ft_strchr(str, c));
}*/