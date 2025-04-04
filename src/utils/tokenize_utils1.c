/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:01:36 by auzou             #+#    #+#             */
/*   Updated: 2025/03/10 14:57:42 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

int	ft_isspace(char a)
{
	if (a == ' ' || a == '\t' || a == '\n')
		return (1);
	return (0);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	check_consecutive_quote(char *input, int pos)
{
	int	is_consecutive;

	is_consecutive = 0;
	if (input[pos] && (is_quote(input[pos])
			|| (!ft_isspace(input[pos]) && !is_special_char(input[pos]))))
	{
		is_consecutive = 1;
		if (is_quote(input[pos])
			&& is_quote(input[pos + 1]) && isspace(input[pos + 2]))
			is_consecutive = 0;
	}
	return (is_consecutive);
}

void	tokenize_util4(t_token **tokens, int *i, char *input)
{
	int		start;
	char	*word;
	int		is_consecutive;
	t_token	*new_token;

	start = *i;
	is_consecutive = 0;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i])
		&& !is_quote(input[*i]))
		(*i)++;
	if (*i > start)
	{
		word = strndup(&input[start], *i - start);
		is_consecutive = check_consecutive_quote(input, *i);
		new_token = create_token(word, TOKEN_WORD);
		new_token->consecutive_quote = is_consecutive;
		add_token(tokens, new_token);
		free(word);
	}
}
