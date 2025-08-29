/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:57:32 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/29 23:05:58 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*map;
	unsigned int	i;

	i = 0;
	map = ft_strdup(s);
	if (!map)
		return (NULL);
	while (map[i])
	{
		map[i] = f(i, map[i]);
		i++;
	}
	return (map);
}

/*int	main(void)
{
	char const *s = "abcde";
	char(*f) = "ft_toupper.c";
}*/