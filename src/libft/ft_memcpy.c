/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/03 14:35:13 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;

	d = (char *)dest;
	s = (char *)src;
	if (!dest && !src)
		return (NULL);
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	char	src[] = "12345abcde";
	memcpy(src + 3, src, 5);
	printf("overlap, origin: %s\n", src);
	
	char	src2[] = "12345abcde";
	char	dest[15] = {0};
	memcpy(dest, src2, 5);
	printf("overlap, origin: %s\n", dest);
	
	char	str[] = "\0";
	ft_memcpy(str + 3, str, 5);
	printf("overlap, new: %s\n", str);

	char	str2[] = "12345abcde";
	char	dest2[15] = {0};
	ft_memcpy(dest2, str2, 5);
	printf("overlap, new: %s\n", dest2);
}*/
