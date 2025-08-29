/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:46:09 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/29 23:22:18 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!*needle)
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while (haystack[i + j] == needle[j] && (i + j) < len)
			{
				j++;
				if (needle[j] == '\0')
				{
					return ((char *)(haystack + i));
				}
			}
		}
		i++;
	}
	return (NULL);
}

/*int	main(void)
{
	size_t		len;
	const char	big[] = "123456789";
	const char	little[] = "567";

	len = 15;
	printf("%s\n", ft_strnstr(big, little, 10));
}*/