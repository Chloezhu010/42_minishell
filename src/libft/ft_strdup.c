/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/11/28 11:52:21 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	int		len;
	char	*new_s;

	len = ft_strlen(s);
	new_s = malloc(len + 1);
	if (new_s == NULL)
		return (NULL);
	return (ft_memcpy(new_s, s, len + 1));
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	char	*origin = "hello world";
	char	*copy = strdup(origin);
	char	*copy2 = ft_strdup(origin);


	printf("%s: %s\n", copy, copy2);
	free(copy);
}*/
