/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:10:57 by auzou             #+#    #+#             */
/*   Updated: 2025/02/27 21:24:55 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

int	check_format_of_pipe(t_token *tokens)
{
	if (!tokens->next || tokens->next->type == TOKEN_PIPE)
	{
		printf("bash: syntax error near unexpected token '|'\n");
		return (0);
	}
	return (1);
}

int	check_format_of_redirect(t_token *tokens)
{
	if (!tokens->next)
		return (printf("bash: syntax error near unexpected token 'newline'\n"), 1);
	else if (tokens->type == TOKEN_REDIRECT_IN
		&& tokens->next->type == TOKEN_REDIRECT_OUT)
		return (printf("bash: syntax error near unexpected token 'newline'\n"), 1);
	else if (tokens->next->type == TOKEN_REDIRECT_IN
		|| tokens->next->type == TOKEN_REDIRECT_OUT
		|| tokens->next->type == TOKEN_REDIRECT_APPEND
		|| tokens->next->type == TOKEN_HEREDOC
		|| tokens->next->type == TOKEN_PIPE)
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			tokens->next->value);
		return (1);
	}
	else if (tokens->next->type != TOKEN_WORD
			&& tokens->next->type != TOKEN_SINGLE_QUOTE // add
			&& tokens->next->type != TOKEN_DOUBLE_QUOTE // add
	)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		return (1);
	}
	return (0);
}

static int	validate_tokens_util(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE && (!check_format_of_pipe(tokens)))
		return (0);
	else if ((tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_APPEND
			|| tokens->type == TOKEN_HEREDOC)
		&& (check_format_of_redirect(tokens)))
	{
		return (0);
	}
	else if (tokens->type == TOKEN_AND || tokens->type == TOKEN_AND_SINGLE)
	{
		if (!tokens->next || tokens->next->type == TOKEN_AND
			|| tokens->next->type == TOKEN_PIPE)
		{
			printf("bash: syntax error near unexpected token `%s'\n",
				tokens->value);
			return (0);
		}
	}
	return (1);
}

int	validate_tokens(t_token *tokens)
{
	if (!tokens)
		return (0);
	while (tokens)
	{
		if (tokens->type == TOKEN_OR)
		{
			printf("bash: syntax error near unexpected token '||'\n");
			return (0);
		}
		if (validate_tokens_util(tokens) == 0)
			return (0);
		tokens = tokens->next;
	}
	return (1);
}