/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:18:15 by czhu              #+#    #+#             */
/*   Updated: 2024/12/06 17:24:51 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_string(char *str)
{
	int	count;

	count = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (*str)
	{
		count += print_char((int) *str);
		str++;
	}
	return (count);
}
/*
int	main()
{
	print_string("hello world\n");
	print_string("");
	print_string("\n");
	print_string(NULL);
	print_string("\n");
	print_string("42\t Coding\n");
	print_string("1234 world\n");
}*/
