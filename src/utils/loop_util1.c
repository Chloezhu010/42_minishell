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

/* helper function to extract a filename that follows a redirect ops */
void	extract_redirect_filename(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_quote(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
		(*i)++;
	if (*i > start)
	{
		word = strndup(&input[start], *i - start);
		add_token(tokens, create_token(word, TOKEN_WORD));
		free(word);
	}
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (is_quote(input[i]) && input[i] == input[i + 1])
			i += 2;
		if (is_special_char(input[i]))
			tokenize_util1(&tokens, &i, input);
		else if (input[i] == '|')
			tokenize_util2(&tokens, &i, input);
		else if (input[i] == '\'' || input[i] == '"')
		{
			if (tokenize_util3(&tokens, &i, input) == 1)
				return (NULL);
		}
		else
			tokenize_util4(&tokens, &i, input);
	}
	return (tokens);
}

void	add_redir(t_cmd *cmd, char *file, int type)
{
	t_redir	*new_redir;
	t_redir	*temp;

	if (!cmd || !file)
		return ;
	update_cmd_files(cmd, file, type);
	new_redir = create_new_redir(file, type);
	if (!new_redir)
		return ;
	if (!cmd->redirects)
		cmd->redirects = new_redir;
	else
	{
		temp = cmd->redirects;
		while (temp->next)
			temp = temp->next;
		temp->next = new_redir;
	}
}
