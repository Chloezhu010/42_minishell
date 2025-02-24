/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/04 13:40:29 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t num_elements, size_t element_size)
{
	void		*ptr;
	size_t		total_size;

	total_size = num_elements * element_size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}
/*
#include <stdio.h>
int	main()
{
	// allocate an array of 5 int, all initialized to 0
	int	*numbers = ft_calloc(0, 10);
	//int	*numbers = calloc(5, 0);


	if (numbers == NULL)
	{
		printf("mem alloc failed\n");
	}

	// print the initial zeroed array
	printf("initial array: \n");
	for (int i = 0; i < 5; i++)
	{
		printf("%d ", numbers[i]);
	}
	printf("\n");

	// free allocated memory
	free(numbers);
}*/