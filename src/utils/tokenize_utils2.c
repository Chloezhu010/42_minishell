/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:20:16 by auzou             #+#    #+#             */
/*   Updated: 2025/03/20 16:43:22 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

void	tokenize_util2(t_token **tokens, int *i, char *input)
{
	char	op[2];

	op[0] = input[*i];
	op[1] = 0;
	add_token(tokens, create_token(op, get_token_type(op)));
	(*i)++;
}

int	tokenize_util3(t_token **tokens, int *i, char *input)
{
	char	quote;
	char	*quoted;
	int		type;

	quote = input[(*i)++];
	quoted = extract_quoted(input, i, quote);
	if (!quoted)
		return (1);
	if (quote == '\'')
		type = TOKEN_SINGLE_QUOTE;
	else
		type = TOKEN_DOUBLE_QUOTE;
	add_token(tokens, create_token(quoted, type));
	free(quoted);
	return (0);
}