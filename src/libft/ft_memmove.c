/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/11/26 12:39:04 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	if (!dest && !src)
		return (NULL);
	if (d > s && d < s + n)
	{
		d += n - 1;
		s += n - 1;
		while (n-- > 0)
			*d-- = *s--;
	}
	else
	{
		while (n-- > 0)
			*d++ = *s++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	char	src[] = "abcdefghijk";
	memmove(src + 3, src , 5);
	printf("overlap scenario, origin: %s\n", src);

	char	str[] = "abcdefghijk";
	ft_memmove(str + 3, str, 5);
	printf("overlap scenario, new: %s\n", str);
}*/
