/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:00:53 by czhu              #+#    #+#             */
/*   Updated: 2025/04/05 17:20:31 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/* helper function for free_sing_token */
static void	handle_free_single_token_error(t_token **head,
	t_token *token_to_free)
{
	if (!head || !*head || !token_to_free)
		return ;
	if (*head == token_to_free)
	{
		*head = token_to_free->next;
		free(token_to_free->value);
		free(token_to_free);
		return ;
	}
}

static void	free_single_token(t_token **head,
	t_token *token_to_free)
{
	t_token	*prev;
	t_token	*current;

	handle_free_single_token_error(head, token_to_free);
    // if (!head || !*head || !token_to_free)
    //     return;
    // if (*head == token_to_free)
    // {
    //     *head = token_to_free->next;
    //     free(token_to_free->value);
    //     free(token_to_free);
    //     return;
    // }
	prev = *head;
	current = prev->next;
	while (current && current != token_to_free)
	{
		prev = current;
		current = current->next;
	}
	if (current)
	{
		prev->next = current->next;
		free(current->value);
		free(current);
	}
}

/* helper function for expand_tokens */
static int	should_remove_dollar_token(t_token *current)
{
	if (current->next && (current->next->type == TOKEN_DOUBLE_QUOTE
			|| current->next->type == TOKEN_SINGLE_QUOTE)
		&& ft_strcmp(current->value, "$") == 0)
		return (1);
	if (ft_strcmp(current->value, "$") == 0 && current->consecutive_quote == 1
		&& current->next != NULL)
		return (1);
	if (ft_strcmp(current->value, "$") == 0 && current->consecutive_quote == 1
		&& current->next == NULL && current->type == TOKEN_WORD)
		return (1);
	return (0);
}

void	expand_tokens(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;
	t_token *next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (should_remove_dollar_token(current))
		{
			free_single_token(&tokens, current);
			current = next;
			continue ;
		}
        // if (current->next && (current->next->type == TOKEN_DOUBLE_QUOTE
		// 	|| current->next->type == TOKEN_SINGLE_QUOTE)
		// 	&& ft_strcmp(current->value, "$") == 0)
		// {
		// 	free_single_token(&tokens, current);
		// 	current = next;
		// 	continue ;
		// }
		// else if (ft_strcmp(current->value, "$") == 0 && current->consecutive_quote == 1
		// 	&& current->next != NULL)
		// {
		// 	free_single_token(&tokens, current);
		// 	current = next;
		// 	continue ;
		// }
		// else if (ft_strcmp(current->value, "$") == 0 && current->consecutive_quote == 1
		// 	&& current->next == NULL && current->type == TOKEN_WORD)
		// {
		// 	free_single_token(&tokens, current);
		// 	current = next;
		// 	continue ;
		// }
		if (current->type == TOKEN_DOUBLE_QUOTE || current->type == TOKEN_WORD)
		{
			expanded = expand_var_instr(current->value, env);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
	current = tokens;
	while (current)
		current = next;
}
