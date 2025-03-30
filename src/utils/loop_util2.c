/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 13:57:37 by auzou             #+#    #+#             */
/*   Updated: 2025/03/23 13:52:53 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"
#include "../../incl/minishell.h"

/* helper function for parse_tokens */
static void	process_word_token(t_cmd **current_cmd, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_WORD
		|| (*tokens)->type == TOKEN_SINGLE_QUOTE
		|| (*tokens)->type == TOKEN_DOUBLE_QUOTE
		|| (*tokens)->type == TOKEN_COMMAND)
		process_token_word(*current_cmd, *tokens);
}

/* helper function for parse_tokens */
static void	process_redirect_token(t_cmd **current_cmd, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_REDIRECT_OUT)
		process_redirection(*current_cmd, tokens, TOKEN_REDIRECT_OUT);
	if ((*tokens)->type == TOKEN_REDIRECT_APPEND)
		process_redirection(*current_cmd, tokens, TOKEN_REDIRECT_APPEND);
	if ((*tokens)->type == TOKEN_REDIRECT_IN)
		process_redirection(*current_cmd, tokens, TOKEN_REDIRECT_IN);
}

/* helper function for parse_tokens */
static void	process_pipe_token(t_cmd **current_cmd, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_PIPE && (*tokens)->next)
	{
		(*current_cmd)->next = create_new_cmd();
		*current_cmd = (*current_cmd)->next;
	}
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_head;
	t_cmd	*current_cmd;

	cmd_head = NULL;
	current_cmd = NULL;
	if (!validate_tokens(tokens))
		return (NULL);
	while (tokens)
	{
		if (!current_cmd)
			current_cmd = create_new_command(&cmd_head);
		process_word_token(&current_cmd, &tokens);
		process_redirect_token(&current_cmd, &tokens);
		process_pipe_token(&current_cmd, &tokens);
		if (tokens->type == TOKEN_HEREDOC)
			process_heredoc(current_cmd, &tokens);
		tokens = tokens->next;
	}
	return (cmd_head);
}
