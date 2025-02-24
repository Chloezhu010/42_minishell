/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/11/26 17:03:49 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	len;

	len = ft_strlen(s);
	while (len >= 0)
	{
		if (s[len] == (char)c)
			return ((char *)&s[len]);
		len--;
	}
	if ((char)c == '\0')
		return ((char *)&s[len]);
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>

int	main()
{
	const char str[] = "hello, world!";
	char *ptr = ft_strrchr(str,'\0');

	if (ptr != NULL)
		printf("last occurrence: %s\n", ptr);
	else
		printf("not found\n");
}*/
