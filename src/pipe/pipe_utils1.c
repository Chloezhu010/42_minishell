/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:12:57 by auzou             #+#    #+#             */
/*   Updated: 2025/03/23 14:17:37 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* init the pipe struct */
void	init_pipe(t_pipe *context)
{
	context->pid_count = 0;
	context->prev_pipe_read = -1;
	context->last_pid = -1;
	context->stdin_backup = dup(STDIN_FILENO);
	context->stdout_backup = dup(STDOUT_FILENO);
	context->stderr_backup = dup(STDERR_FILENO);
	context->pipefd[0] = -1;
	context->pipefd[1] = -1;
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

/* create pipe and handle error */
int	create_cmd_pipe(t_pipe *ctx, t_env *env)
{
	if (pipe(ctx->pipefd) == -1)
	{
		perror("pipe");
		exit_status(env, 1);
		return (1);
	}
	return (0);
}

/* wrapper function of dup and handle error */
int	ft_dup(int fd, int *backup_ptr, char *name)
{
	*backup_ptr = dup(fd);
	if (*backup_ptr == -1)
	{
		perror(name);
		return (1);
	}
	return (0);
}

/* wrapper function of dup2 and handle error */
int	ft_dup2(int oldfd, int newfd, int backup, char *name)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror(name);
		close(backup);
		return (1);
	}
	return (0);
}
