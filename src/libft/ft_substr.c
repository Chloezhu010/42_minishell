/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/04 11:58:07 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_start_error(void)
{
	char	*str;

	str = malloc(1);
	if (!str)
		return (NULL);
	str[0] = '\0';
	return (str);
}

static size_t	ft_strnlen(char const *s, size_t maxlen)
{
	size_t	i;

	i = 0;
	while (s[i] && i < maxlen)
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	s_len;
	size_t	copy_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (s_len <= start)
		return (ft_start_error());
	copy_len = ft_strnlen(s + start, len);
	ptr = malloc(sizeof(char) * (copy_len + 1));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s + start, copy_len + 1);
	return (ptr);
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	printf("%s\n", ft_substr("tripouille", 0, 18456789456));
	printf("%s\n", ft_substr("tripouille", 1, 1));
	printf("%s\n", ft_substr("tripouille", 100, 1));
	printf("%s\n", ft_substr("tripouille", 0, 0));
}*/
