/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:42:59 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/30 18:04:06 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;
	size_t	i;
	char	*str;
	size_t	size;

	i = 0;
	if (!s)
		return (NULL);
	slen = ft_strlen((char *)s);
	if (start >= slen)
		return (ft_strdup(""));
	if ((slen - start) <= len)
		size = slen - start;
	else
		size = len;
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	while (s[i + start] && i < len && start < slen)
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = 0;
	return (str);
}

/*int	main(void)
{
	unsigned int start = 4;
	size_t len = 6;
	char const *s = "coucou toi";
	char *str = ft_substr(s, start, len);
	printf("%s", str);
}*/