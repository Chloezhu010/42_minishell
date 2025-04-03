/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:00:41 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 14:09:58 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"
#include "../../incl/minishell.h"

t_cmd	*create_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(64, sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->fd_in = 0;
	cmd->redirects = NULL;
	cmd->in_pipe = 0;
	cmd->expand_heredoc = 1;
	cmd->next = NULL;
	return (cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_cmd	*next;
	int		i;
	
	if (!cmds)
		return ;
	tmp = cmds;
	while (tmp)
	{
		next = tmp->next;
		i = 0;
		if (tmp->args)
		{
			while (tmp->args[i])
			{
				free(tmp->args[i]);
				tmp->args[i] = NULL;
				i++;
			}
			free(tmp->args);
			tmp->args = NULL;
		}
		if (tmp->infile)
        {
            free(tmp->infile);
            tmp->infile = NULL;
        }
        
        if (tmp->outfile)
        {
            free(tmp->outfile);
            tmp->outfile = NULL;
        }
        
        if (tmp->delimiter)
        {
            free(tmp->delimiter);
            tmp->delimiter = NULL;
        }
        free_redir(tmp->redirects);
        tmp->redirects = NULL;
		tmp->next = NULL;
		free(tmp);
		tmp = next;
	}
}

void	check_format_command(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if ((strcmp(tokens->value, "cd") == 0)
				|| (strcmp(tokens->value, "ls") == 0)
				|| (strcmp(tokens->value, "cp") == 0)
				|| (strcmp(tokens->value, "mv") == 0)
				|| (strcmp(tokens->value, "rm") == 0)
				|| (strcmp(tokens->value, "mkdir") == 0)
				|| (strcmp(tokens->value, "rmdir") == 0)
				|| (strcmp(tokens->value, "man") == 0)
				|| (strcmp(tokens->value, "more") == 0)
				|| (strcmp(tokens->value, "cat") == 0)
				|| (strcmp(tokens->value, "echo") == 0)
				|| (strcmp(tokens->value, "grep") == 0)
				|| (strcmp(tokens->value, "pwd") == 0)
				|| (strcmp(tokens->value, "export") == 0)
				|| (strcmp(tokens->value, "unset") == 0)
				|| (strcmp(tokens->value, "env") == 0)
				|| (strcmp(tokens->value, "exit") == 0))
				tokens->type = TOKEN_COMMAND;
		}
		tokens = tokens->next;
	}
}
