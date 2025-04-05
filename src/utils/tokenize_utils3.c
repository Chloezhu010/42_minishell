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

/* helper function for tokenize_util1 */
static void	add_token_and_extract(t_token **tokens,
	char *input, int *i, char *op)
{
	t_token	*new_token;

	new_token = create_token(op, get_token_type(op));
	add_token(tokens, new_token);
	(*i)++;
	extract_redirect_filename(input, i, tokens);
}

/* helper function for tokenize_util1 */
static void	handle_double_operator(t_token **tokens,
	int *i, char *input, char op[3])
{
	op[1] = input[++(*i)];
	add_token_and_extract(tokens, input, i, op);
}

/* helper function for tokenize_util1 */
static void	handle_add_operator(t_token **tokens,
	int *i, char op[3])
{
	t_token	*new_token;

	op[1] = '&';
	(*i)++;
	new_token = create_token(op, get_token_type(op));
	add_token(tokens, new_token);
	(*i)++;
}

void	tokenize_util1(t_token **tokens, int *i, char *input)
{
	char	op[3];
	t_token	*new_token;

	op[0] = input[*i];
	op[1] = 0;
	op[2] = 0;
	new_token = NULL;
	if ((input[*i] == '<' || input[*i] == '>') && input[*i + 1] == input[*i])
		handle_double_operator(tokens, i, input, op);
	else if (input[*i] == '<' || input[*i] == '>')
		add_token_and_extract(tokens, input, i, op);
	else if (input[(*i)] == '&' && input[*i + 1] == '&')
		handle_add_operator(tokens, i, op);
	else
	{
		new_token = create_token(op, get_token_type(op));
		add_token(tokens, new_token);
		(*i)++;
	}
}
