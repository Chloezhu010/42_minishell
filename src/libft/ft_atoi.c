/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/04 12:25:37 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	negative;
	int	i;
	int	result;

	i = 0;
	negative = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			negative = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - '0';
		i++;
	}
	return (result * negative);
}
/*
#include <stdio.h>
#include <stdlib.h>
int	main()
{
	char	str1[] = "+-123";
	char	str2[] = "   + 456";
	char	str3[] = "   +456";
	char	str4[] = "abd123";
	char	str5[] = "2147483647";
	char	str6[] = "-2147483648";
	char	str7[] = "++47";
	char	str8[] = "\t+\f1234";
	char	str9[] = "+\t\v\f\r\n\f1234";

	printf("%d: %d\n", atoi(str1), ft_atoi(str1));
	printf("%d: %d\n", atoi(str2), ft_atoi(str2));
	printf("%d: %d\n", atoi(str3), ft_atoi(str3));
	printf("%d: %d\n", atoi(str4), ft_atoi(str4));
	printf("%d: %d\n", atoi(str5), ft_atoi(str5));
	printf("%d: %d\n", atoi(str6), ft_atoi(str6));
	printf("%d: %d\n", atoi(str7), ft_atoi(str7));
	printf("%d: %d\n", atoi(str8), ft_atoi(str8));
	printf("%d: %d\n", atoi(str9), ft_atoi(str9));
}*/
