/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:18:21 by czhu              #+#    #+#             */
/*   Updated: 2024/12/05 11:49:12 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	count_word(const char *s, char c)
{
	int	i;
	int	in_word;
	int	count;

	i = 0;
	in_word = 0;
	count = 0;
	while (s[i])
	{
		if ((s[i] != c) && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

void	ft_freeup(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}

static char	*ft_alloc_word(const char *s, char c, int *i)
{
	int		j;
	char	*word;

	j = 0;
	while (s[*i + j] && s[*i + j] != c)
		j++;
	word = malloc(sizeof(char) * (j + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + *i, j + 1);
	*i += j;
	return (word);
}

static int	ft_putsplit(char **split, const char *s, char c)
{
	int	i;
	int	j;
	int	count_str;

	count_str = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			split[count_str] = ft_alloc_word(s, c, &i);
			if (split[count_str] == NULL)
			{
				j = 0;
				while (j < count_str)
					free(split[j++]);
				return (0);
			}
			count_str++;
		}
	}
	split[count_str] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		count;
	int		i;
	char	**res;

	if (!s)
		return (NULL);
	count = count_word(s, c);
	res = (char **) malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i <= count)
	{
		res[i] = NULL;
		i++;
	}
	if (ft_putsplit(res, s, c) == 0)
	{
		free(res);
		return (NULL);
	}
	return (res);
}
// #include <stdio.h>
// int	main()
// {
// 	char s[] = "^^^1^^2a,^^^^3^^^^--h^^^^";
// 	char c = '^';
// 	char **split_str = ft_split(s, c);
// 	int count = count_word(s, c);
// 	printf("test count word: %d\n", count);
// 	int i = 0;
// 	while (s[i] && i < count)
// 	{
// 		printf("result: %s\n", split_str[i]);
// 		i++;
// 	}
// 	ft_freeup(split_str);
// }
