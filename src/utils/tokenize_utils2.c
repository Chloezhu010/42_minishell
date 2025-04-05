/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:20:16 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 17:59:24 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

void	tokenize_util2(t_token **tokens, int *i, char *input)
{
	char	op[2];
	t_token	*new_token;

	op[0] = input[*i];
	op[1] = 0;
	new_token = create_token(op, get_token_type(op));
	add_token(tokens, new_token);
	(*i)++;
}

static int	is_consecutive_quote(char *input, int *i)
{
	int	is_consecutive;

	is_consecutive = 0;
	if (input[*i] && (is_quote(input[*i])
			|| ft_isalpha(input[*i]) || ft_isdigit(input[*i])))
	{
		is_consecutive = 1;
		if (is_quote(input[*i])
			&& is_quote(input[*i + 1]) && !isspace(input[*i + 2]))
			is_consecutive = 1;
	}
	if (input[*i] && !isspace(input[*i]))
		is_consecutive = 1;
	return (is_consecutive);
}

int	tokenize_util3(t_token **tokens, int *i, char *input)
{
	char	quote;
	char	*quoted;
	int		type;
	int		is_consecutive;
	t_token	*new_token;

	is_consecutive = 0;
	quote = input[(*i)++];
	quoted = extract_quoted(input, i, quote);
	if (!quoted)
	{
		free_tokens(*tokens);
		return (1);
	}
	is_consecutive = is_consecutive_quote(input, i);
	if (quote == '\'')
		type = TOKEN_SINGLE_QUOTE;
	else
		type = TOKEN_DOUBLE_QUOTE;
	new_token = create_token(quoted, type);
	new_token->consecutive_quote = is_consecutive;
	add_token(tokens, new_token);
	free(quoted);
	return (0);
}
