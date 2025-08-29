/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:45:45 by mlemerci          #+#    #+#             */
/*   Updated: 2024/12/05 08:43:02 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_size;
	size_t	src_size;

	if ((!src || !dst) && size == 0)
		return (0);
	i = 0;
	dest_size = ft_strlen(dst);
	src_size = ft_strlen(src);
	if (dest_size >= size)
		return (src_size + size);
	while (src[i] != '\0' && i < (size - dest_size - 1))
	{
		dst[dest_size + i] = src[i];
		i++;
	}
	dst[dest_size + i] = '\0';
	return (src_size + dest_size);
}

/*
int main() {
    char src[] = "World!";
    char dest[20] = "Hello, ";
    printf("Avant strlcat: dest = \"%s\", src = \"%s\"\n", dest, src);
    size_t n = strlcat(dest, src, sizeof(dest));
    printf("Après strlcat: dest = \"%s\"\n", dest);
    printf("Taille totale de la chaîne résultante : %zu\n", n);

    return 0;
}
*/