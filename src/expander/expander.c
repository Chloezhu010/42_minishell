/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:57:23 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 17:57:56 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/* Find the end position of a variable name starting after $ */
static char	*find_var_end(char *var_start)
{
	char	*var_end;

	var_end = var_start + 1;
	if (*var_end && (*var_end == '?' || (*var_end >= '0' && *var_end <= '9')))
		return (var_end + 1);
	if ((*var_end >= 'A' && *var_end <= 'Z') || *var_end == '_')
	{
		while ((*var_end >= 'A' && *var_end <= 'Z')
			|| *var_end == '_'
			|| (*var_end >= '0' && *var_end <= '9'))
			var_end++;
	}
	return (var_end);
}

char	*extract_var_name(char *str)
{
	char	*var_start;
	char	*var_end;

	var_start = ft_strchr(str, '$');
	if (!var_start)
		return (NULL);
	var_end = find_var_end(var_start);
	return (strndup(var_start, var_end - var_start));
}

/* expand var in a str
	- set up an empty str res
	- loop through the input str
		- if encounter $
			- call handle_var_expansion
		- if not (a regular string)
			- call handle_regular_char
*/
char	*expand_var_instr(char *input, t_env *env)
{
	char	*res;
	char	*ptr;

	ptr = input;
	res = ft_strdup("");
	while (*ptr)
	{
		if (*ptr == '$')
			res = handle_var_expansion(res, &ptr, env);
		else
		{
			res = handle_regular_char(res, *ptr);
			ptr++;
		}
	}
	return (res);
}

/*  expand var in the token array
	- skip single quoted string
	- handle double quoted string or normal string
		- expand var using expand_var_instr
		- if expand successfully
			- free current token value
			- update with the expanded value
		- move to the next token
*/
int	merge_token_with_next(t_token *current)
{
	t_token	*next;
	char	*combined;

	if (!current->consecutive_quote)
		return (0);
	next = current->next;
	combined = ft_strjoin(current->value, next->value);
	if (combined)
	{
		free(current->value);
		current->value = combined;
		current->consecutive_quote = next->consecutive_quote;
		current->next = next->next;
		free(next->value);
		free(next);
		return (1);
	}
	return (0);
}

void	merge_consecutive_tokens(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return ;
	current = tokens;
	while (current && current->next)
	{
		if (merge_token_with_next(current))
			continue ;
		current = current->next;
	}
}
