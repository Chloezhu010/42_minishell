/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/03 11:31:58 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(-n, fd);
	}
	else if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + '0', fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
}
/*
#include <stdio.h>
int	main()
{
	int	n1 = -42;
	int	n2 = 164189;
	int	n3 = 0;
	int	n4 = 2147483647;
	int	n5 = -2147483648;

	ft_putnbr_fd(n1, 1);
	write(1, "\n", 1);
	ft_putnbr_fd(n2, 1);
	write(1, "\n", 1);
	ft_putnbr_fd(n3, 1);
	write(1, "\n", 1);
	ft_putnbr_fd(n4, 1);
	write(1, "\n", 1);
	ft_putnbr_fd(n5, 1);
}*/
