/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:43:58 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/29 23:08:03 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_pars(char c, char const *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		len;
	char	*dest;

	start = 0;
	while (s1[start] && ft_pars(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_pars(s1[end - 1], set))
		end--;
	len = end - start;
	dest = ft_substr(s1, start, len);
	if (!dest)
		return (NULL);
	dest[len] = '\0';
	return (dest);
}

/*int	main(void)
{
	char const *set = "ab";
	char const *s1 = "aaaabcommebnt ca vaaab";
	printf("%s", ft_strtrim(s1, set));
}*/