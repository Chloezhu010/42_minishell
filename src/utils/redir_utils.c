/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:00:06 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:00:10 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"
#include "../../incl/minishell.h"

t_redir	*create_new_redir(char *file, int type)
{
	t_redir	*redir;

	redir = ft_malloc(sizeof(t_redir));
	redir->file = ft_strdup(file);
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

void	update_cmd_files(t_cmd *cmd, char *file, int type)
{
	if (type == TOKEN_REDIRECT_IN)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup(file);
	}
	else if (type == TOKEN_REDIRECT_APPEND || type == TOKEN_REDIRECT_OUT)
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = ft_strdup(file);
		cmd->append = (type == TOKEN_REDIRECT_APPEND);
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir;
		free(redir->file);
		redir = redir->next;
		free(temp);
	}
}
