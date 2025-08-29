/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlemerci <mlemerci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:45:04 by mlemerci          #+#    #+#             */
/*   Updated: 2024/11/30 18:19:59 by mlemerci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int nb, int fd)
{
	long int	nbl;

	nbl = nb;
	if (fd < 0)
		return ;
	if (nbl < 0)
	{
		ft_putchar_fd('-', fd);
		nbl = -nbl;
	}
	if (nbl > 9)
	{
		ft_putnbr_fd(nbl / 10, fd);
		ft_putchar_fd(nbl % 10 + '0', fd);
	}
	else
		ft_putchar_fd(nbl + '0', fd);
}
