/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:05:25 by auzou             #+#    #+#             */
/*   Updated: 2025/03/31 13:09:07 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/**
 * get_token_type_name - Returns a string representation of token type
 * @type: Token type code
 * 
 * Return: String representation of the token type
 */
static char	*get_token_type_name(int type)
{
	switch (type)
	{
		case TOKEN_WORD:
			return ("WORD");
		case TOKEN_PIPE:
			return ("PIPE");
		case TOKEN_OR:
			return ("OR");
		case TOKEN_REDIRECT_IN:
			return ("REDIRECT_IN");
		case TOKEN_REDIRECT_OUT:
			return ("REDIRECT_OUT");
		case TOKEN_REDIRECT_APPEND:
			return ("REDIRECT_APPEND");
		case TOKEN_HEREDOC:
			return ("HEREDOC");
		case TOKEN_SINGLE_QUOTE:
			return ("SINGLE_QUOTE");
		case TOKEN_DOUBLE_QUOTE:
			return ("DOUBLE_QUOTE");
		case TOKEN_AND:
			return ("AND");
		case TOKEN_AND_SINGLE:
			return ("AND_SINGLE");
		case TOKEN_COMMAND:
			return ("COMMAND");
		default:
			return ("UNKNOWN");
	}
}

/**
 * print_tokens - Prints the linked list of tokens
 * @tokens: Pointer to the first token in the list
 * 
 * This function displays all tokens in the linked list,
 * showing their values and types.
 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		count;

	if (!tokens)
	{
		printf("No tokens to display\n");
		return;
	}
	
	printf("\n--- Tokens ---\n");
	current = tokens;
	count = 0;
	while (current)
	{
		printf("[%d] Type: %-15s Value: ", count, get_token_type_name(current->type));
		if (current->value)
			printf("\"%s\"", current->value);
		else
			printf("(null)");
		
		if (current->consecutive_quote > 0)
			printf(" (consecutive_quote: %d)", current->consecutive_quote);
		
		printf("\n");
		current = current->next;
		count++;
	}
	printf("-------------\n\n");
}


/**
 * print_redirects - Prints all redirections for a command
 * @redirects: Pointer to the first redirection in the list
 */
static void	print_redirects(t_redir *redirects)
{
	t_redir	*current;
	int		count;
	
	if (!redirects)
	{
		printf("    No redirections\n");
		return;
	}
	
	printf("    Redirections:\n");
	current = redirects;
	count = 0;
	while (current)
	{
		printf("    [%d] Type: ", count);
		switch (current->type)
		{
			case TOKEN_REDIRECT_IN:
				printf("REDIR_IN");
				break;
			case TOKEN_REDIRECT_OUT:
				printf("REDIR_OUT");
				break;
			case TOKEN_REDIRECT_APPEND:
				printf("REDIR_APPEND");
				break;
			case TOKEN_HEREDOC:
				printf("HEREDOC");
				break;
			default:
				printf("UNKNOWN (%d)", current->type);
		}
		
		printf(", File: ");
		if (current->file)
			printf("\"%s\"", current->file);
		else
			printf("(null)");
		printf("\n");
		
		current = current->next;
		count++;
	}
}

/**
 * print_args - Prints all arguments for a command
 * @args: Array of argument strings
 */
static void	print_args(char **args)
{
	int	i;
	
	if (!args || !args[0])
	{
		printf("    No arguments\n");
		return;
	}
	
	printf("    Arguments:\n");
	i = 0;
	while (args[i])
	{
		printf("    [%d] \"%s\"\n", i, args[i]);
		i++;
	}
}

/**
 * print_cmds - Prints the linked list of commands
 * @cmds: Pointer to the first command in the list
 * 
 * This function displays all commands in the linked list,
 * showing their arguments, redirections, and other properties.
 */
void	print_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	int		count;

	if (!cmds)
	{
		printf("No commands to display\n");
		return;
	}
	
	printf("\n--- Commands ---\n");
	current = cmds;
	count = 0;
	while (current)
	{
		printf("Command [%d]:\n", count);
		
		print_args(current->args);
		
		printf("    Infile: ");
		if (current->infile)
			printf("\"%s\"", current->infile);
		else
			printf("(null)");
		printf(", fd_in: %d\n", current->fd_in);
		
		printf("    Outfile: ");
		if (current->outfile)
			printf("\"%s\"", current->outfile);
		else
			printf("(null)");
		printf(", append: %d\n", current->append);
		
		printf("    Heredoc: %d, Delimiter: ", current->heredoc);
		if (current->delimiter)
			printf("\"%s\"", current->delimiter);
		else
			printf("(null)");
		printf("\n");
		
		printf("    In pipe: %d\n", current->in_pipe);
		
		print_redirects(current->redirects);
		
		printf("\n");
		current = current->next;
		count++;
	}
	printf("----------------\n\n");
}
