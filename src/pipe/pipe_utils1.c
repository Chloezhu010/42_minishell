/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:12:57 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:20:12 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* pipe function wrapper */
void	ft_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
}

/* helper function: check if input files exist for a cmd
    - return 1 for error
    - return 0 for success
*/
int	check_input_file(t_cmd *cmd, t_env *env)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror("minishell");
				env->exit_status = 1;
				return (1);
			}
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

/* helper function: create output files for a cmd */
int	create_output_file(t_cmd *cmd, t_env *env)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_OUT
			|| redir->type == TOKEN_REDIRECT_APPEND)
		{
			if (redir->type == TOKEN_REDIRECT_APPEND)
				fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("minishell");
				env->exit_status = 1;
				return (1);
			}
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

/* init the pipe struct */
void	init_pipe(t_pipe *context)
{
	context->pid_count = 0;
	context->prev_pipe_read = -1;
	context->last_pid = -1;
	context->stdin_backup = dup(STDIN_FILENO);
	context->stdout_backup = dup(STDOUT_FILENO);
	context->stderr_backup = dup(STDERR_FILENO);
}

void	mark_pipeline_cmd(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		current->in_pipe = 1;
		current = current->next;
	}
}
