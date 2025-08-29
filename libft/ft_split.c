/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:59:30 by mlemerci          #+#    #+#             */
/*   Updated: 2024/12/02 03:35:37 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbrwrds(char const *s, char c)
{
	int	i;
	int	wrd;

	i = 0;
	wrd = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			wrd++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (wrd);
}

int	ft_sizewrd(char c, char const *wrd)
{
	int	i;

	i = 0;
	while (wrd[i] && wrd[i] != c)
		i++;
	return (i);
}

void	*ft_clear(int j, char **res)
{
	while (j > 0)
		free(res[--j]);
	free(res);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * (ft_nbrwrds(s, c) + 1));
	if (!res)
		return (NULL);
	while (s[i] && res)
	{
		if (s[i] == c)
			i++;
		else
		{
			res[j] = malloc(sizeof(char) * (ft_sizewrd(c, s + i) + 1));
			if (!res[j])
				return (ft_clear(j, res));
			ft_strlcpy(res[j], s + i, ft_sizewrd(c, s + i) + 1);
			i += ft_sizewrd(c, s + i);
			j++;
		}
	}
	res[j] = NULL;
	return (res);
}

/*void	ft_print_split(char **res)
{
	int	i;

	i = 0;
	while (res[i] != NULL)
	{
		printf("%s\n", res[i]);
		i++;
	}
}

int	main(void)
{
	char str[] = "Hello world this is a test";
	char delimiter = ' ';

	char **result = ft_split(str, delimiter);
	ft_print_split(result);
	int i = 0;
	while (result[i] != NULL)
	{
		free(result[i]);
		i++;
	}
	free(result);
}*/