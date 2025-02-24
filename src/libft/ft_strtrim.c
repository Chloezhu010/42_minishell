/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/02 13:18:30 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	check_inset(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	end;
	char	*res;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	end = ft_strlen(s1);
	while (s1[i] && check_inset(s1[i], set))
		i++;
	while (end > i && check_inset(s1[end - 1], set))
		end --;
	res = malloc(end - i + 1);
	if (!res)
		return (NULL);
	ft_strncpy(res, &s1[i], end - i);
	return (res);
}
/*
#include <stdio.h>

int	main()
{
	char	*newstring = ft_strtrim(" xxx ", "+-");
	printf("%s\n", newstring);
	free(newstring);
	return 0;
}*/
