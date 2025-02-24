/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/11/29 18:16:27 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
#include <stdio.h>
#include <ctype.h>

void	ft_uplow(unsigned int index, char *c)
{
	if (index % 2 == 0)
		*c = toupper(*c);
	else
		*c = tolower(*c);

}

int	main()
{
	char	str1[] = "hello, world!";
	printf("before: %s\n", str1);
	ft_striteri(str1, ft_uplow);
	printf("after: %s\n", str1);
}*/
