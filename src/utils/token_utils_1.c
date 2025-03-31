/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:03:41 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 13:58:17 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

char	*extract_quoted(char *input, int *i, char quote)
{
	char	*quoted;

	if (input[*i] == input[*i - 1])
		return (NULL);
	quoted = extract_single_quoted(input, i, quote);
	if (!quoted)
		return (NULL);
	return (quoted);
}

t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = strdup(value);
	token->type = type;
	token->consecutive_quote = 0;
	token->next = NULL;
	return (token);
}

int	get_token_type(char *s)
{
	if (strcmp(s, "|") == 0)
		return (TOKEN_PIPE);
	if (strcmp(s, "||") == 0)
		return (TOKEN_OR);
	if (strcmp(s, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (strcmp(s, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (strcmp(s, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	if (strcmp(s, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (strcmp(s, "&&") == 0)
		return (TOKEN_AND);
	if (strcmp(s, "&") == 0)
		return (TOKEN_AND_SINGLE);
	return (TOKEN_WORD);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		free(tokens->value);
		tokens = tokens->next;
		free(tmp);
	}
}
