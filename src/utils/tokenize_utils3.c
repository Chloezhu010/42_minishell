/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:59:13 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 12:48:10 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

/* helper function of strndup */
char *ft_strndup(char *str, size_t n)
{
	size_t len;
	size_t i;
	char *dup;

	len = 0;
	i = 0;
	while (str[len] && len < n)
		len++;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	while(i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i]='\0';
	return (dup);
}

static int	tokenize_util4_util1(char *input, int *start, char **word, int *i)
{
	char	quote;
	char	*new_part;
	char	*temp;

	while (input[*i] && is_quote(input[*i]))
	{
		quote = input[*i];
		*start = ++(*i);
		while (input[*i] && input[*i] != quote)
			(*i)++;
		if (!input[*i])
		{
			printf("quote need be closed");
			return (-1);
		}
		new_part = ft_strndup(&input[*start], (*i) - (*start));
		temp = ft_strjoin(*word, new_part);
		free(new_part);
		free(*word);
		*word = temp;
		(*i)++;
	}
	return (0);
}

static int	tokenize_util4_append_word(char *input, int *i, char **word)
{
	int	start;
	char *temp_str;
	char *result;

	if (is_quote(input[*i]))
	{
		start = *i;
		if (tokenize_util4_util1(input, &start, word, i) == -1)
			return (-1);
	}
	else
	{
		start = *i;
		while (input[*i] && !ft_isspace(input[*i])
			&& !is_special_char(input[*i]) && !is_quote(input[*i]))
			(*i)++;
		// *word = ft_strjoin(*word, ft_strndup(&input[start], (*i) - start));
		temp_str = ft_strndup(&input[start], (*i) - start);
		if (!temp_str)
			return (-1);
		result = ft_strjoin(*word, temp_str);
		free(temp_str);
		free(*word);
		*word = result;
	}
	return (0);
}

static int	tokenize_util4_parse_word(char *input, int *i, char **word)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_special_char(input[*i]) && !is_quote(input[*i]))
		(*i)++;
	if (*i > start)
		*word = ft_strndup(&input[start], (*i) - start);
	else
		return (0);
	while (input[*i] && (!ft_isspace(input[*i]) && !is_special_char(input[*i])))
	{
		if (tokenize_util4_append_word(input, i, word) == -1)
			return (-1);
	}
	return (1);
}

void	tokenize_util4(t_token **tokens, int *i, char *input)
{
	char	*word;

	word = NULL;
	if (tokenize_util4_parse_word(input, i, &word) == 1)
	{
		add_token(tokens, create_token(word, TOKEN_WORD));
		free(word);
	}
	if (input[*i])
		(*i)++;
}
