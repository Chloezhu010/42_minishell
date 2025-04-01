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

	if (!file)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

void	update_cmd_files(t_cmd *cmd, char *file, int type)
{
	char	*new_file;

	if (!cmd || !file)
		return ;
	new_file = ft_strdup(file);
	if (!new_file)
		return ;
	if (type == TOKEN_REDIRECT_IN)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = new_file;
	}
	else if (type == TOKEN_REDIRECT_APPEND || type == TOKEN_REDIRECT_OUT)
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = new_file;
		cmd->append = (type == TOKEN_REDIRECT_APPEND);
	}
	else
		free(new_file);
}

void	free_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		free(redir->file);
		free(redir);
		redir = temp;
	}
}

int	handle_redirect(t_cmd *cmd, int *stdin_backup,
	int *stdout_backup, t_env *env)
{
	if (!cmd->in_pipe && process_redirect(cmd, env))
		return (1);
	if (handle_input_redirect(cmd, stdin_backup, env) == -1
		|| handle_output_redirect(cmd, stdout_backup, env) == -1)
	{
		exit_status(env, 1);
		return (1);
	}
	return (0);
}
