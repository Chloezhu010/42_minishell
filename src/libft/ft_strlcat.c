/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/03 15:38:04 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dst[i] && i < siz)
		i++;
	while (src[j] && (i + j + 1) < siz)
	{
		dst[i + j] = src[j];
		j++;
	}
	if (i < siz)
		dst[i + j] = '\0';
	return (i + ft_strlen(src));
}
/*
#include <stdio.h>
int	main()
{
	char	src[] = "world!";
	char	dst[10] = "hello, ";
	size_t	result = ft_strlcat(dst, src, -1);

	printf("Result: %s, Total length: %zu\n", dst, result);
}*/
