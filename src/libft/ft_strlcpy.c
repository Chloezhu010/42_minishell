/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/11/26 14:31:23 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen(src);
	if (siz == 0)
		return (src_len);
	if (src_len < siz - 1)
	{
		while (i < src_len)
		{
			dst[i] = src[i];
			i++;
		}
	}
	else
	{
		while (i < siz - 1)
		{
			dst[i] = src[i];
			i++;
		}
	}
	dst[i] = '\0';
	return (src_len);
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	char src[] = "hello world";
	char dest[20];
	size_t len = strlcpy(dest, src, sizeof(dest));

	printf("source: %s\n", src);
	printf("dest: %s\n", dest);
	printf("length of src: %zu\n", len);

	char src2[] = "hello world";
	char dest2[5];
	size_t len2 = ft_strlcpy(dest2, src2, sizeof(dest2));

	printf("source: %s\n", src2);
	printf("dest: %s\n", dest2);
	printf("length of src: %zu\n", len2);
}*/
