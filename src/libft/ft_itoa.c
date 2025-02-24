/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/11/29 16:11:02 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	check_negative(long n)
{
	if (n >= 0)
		return (0);
	else
		return (1);
}

static int	count_digit(long n)
{
	int	count;
	int	negative;

	count = 0;
	negative = check_negative(n);
	if (negative == 1)
		n = -n;
	while (n / 10 >= 1)
	{
		count++;
		n = n / 10;
	}
	return (count + 1 + negative);
}

char	*ft_itoa(int n)
{
	long	num;
	char	*res;
	int		len;

	num = (long)n;
	len = count_digit(num);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (num < 0)
	{
		res[0] = '-';
		num = -num;
	}
	else if (num == 0)
		res[0] = '0';
	res[len + 1] = '\0';
	while (num != 0)
	{
		res[len - 1] = num % 10 + '0';
		num /= 10;
		len--;
	}
	return (res);
}
/*
#include <stdio.h>
int	main()
{
	int	n1 = -42;
	int	n2 = 0;
	int	n3 = 2147483647;
	int	n4 = -2147483648;
	int	n5 = -10;
	int	n6 = -100;
	int	n7 = 300;
	
	printf("count: %d\n", count_digit(n1));
	printf("count: %d\n", count_digit(n2));
	printf("count: %d\n", count_digit(n3));
	printf("count: %d\n", count_digit(n4));
	printf("count: %d\n", count_digit(n5));
	printf("count: %d\n", count_digit(n6));
	printf("count: %d\n", count_digit(n7));

	printf("%s\n", ft_itoa(n1));
	printf("%s\n", ft_itoa(n2));
	printf("%s\n", ft_itoa(n3));
	printf("%s\n", ft_itoa(n4));
	printf("%s\n", ft_itoa(n5));
	printf("%s\n", ft_itoa(n6));
	printf("%s\n", ft_itoa(n7));
}*/