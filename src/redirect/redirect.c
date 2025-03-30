/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:21:07 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:38:52 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/* process all redirect in order (from left to right) 
    - check if files exist/ can be created, and handle permissions
    - return 0 on success, 1 on error
*/

int	handle_input_redirect1(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirect1(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_REDIRECT_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (1);
	}
	close(fd);
	return (0);
}

void	handle_redirect_error1(int fd, int *stdout_backup, t_env *env)
{
	perror("minishell");
	close(fd);
	close(*stdout_backup);
	env->exit_status = 1;
}

int	handle_output_redirect_result(t_cmd *cmd, t_env *env, int result)
{
	if (result == 1)
	{
		if (!cmd->in_pipe)
			env->exit_status = 1;
		return (1);
	}
	return (0);
}

int	process_redirect(t_cmd *cmd, t_env *env)
{
	t_redir	*redir;
	int		result;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			result = handle_input_redirect1(redir);
			if (handle_output_redirect_result(cmd, env, result) == 1)
				return (1);
		}
		else if (redir->type == TOKEN_REDIRECT_APPEND
			|| redir->type == TOKEN_REDIRECT_OUT)
		{
			result = handle_output_redirect1(redir);
			if (handle_output_redirect_result(cmd, env, result) == 1)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
