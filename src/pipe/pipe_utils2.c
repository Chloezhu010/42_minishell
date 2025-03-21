/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:14:04 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:20:28 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* create pipe and handle error */
int	create_cmd_pipe(t_pipe *ctx, t_env *env)
{
	if (pipe(ctx->pipefd) == -1)
	{
		perror("pipe");
		env->exit_status = 1;
		return (1);
	}
	return (0);
}

/* handle redirect error
    - if there is a next cmd
        - close the read & write end of the pipe
        - exit 1
*/
void	handle_redirect_error(t_cmd *cmd, t_pipe *ctx, int redirect_error)
{
	if (redirect_error)
	{
		if (cmd->next)
		{
			close(ctx->pipefd[0]);
			close(ctx->pipefd[1]);
		}
		exit(1);
	}
}

/* handle redirect stderr to /dev/null for non-last cmd */
void	handle_stderr_redirect(t_cmd *cmd)
{
	int	null_fd;

	if (cmd->next)
	{
		null_fd = open("/dev/null", O_WRONLY);
		if (null_fd != -1)
		{
			dup2(null_fd, STDERR_FILENO);
			close(null_fd);
		}
	}
}

/* setup pipe input
    - handle input from pipeline
        - if the current cmd is part of a pipeline, read from its prev cmd
        - current cmd read its input from the prev cmd
        - close the prev_pipe_read
    - else handle input redirect
        - if redirect fails, close both read & write end of pipe
    - return 1 on error, 0 on success
*/
int	setup_pipe_input(t_cmd *cmd, t_pipe *ctx, int *stdin_backup, t_env *env)
{
	if (cmd->infile || (cmd->heredoc && cmd->fd_in > 0))
	{
		if (handle_input_redirect(cmd, stdin_backup, env) == -1)
		{
			if (cmd->next)
			{
				close(ctx->pipefd[0]);
				close(ctx->pipefd[1]);
			}
			return (1);
		}
	}
	else if (ctx->prev_pipe_read != -1)
	{
		dup2(ctx->prev_pipe_read, STDIN_FILENO);
		close(ctx->prev_pipe_read);
	}
	return (0);
}

/* setup pipe output
    - handle output for pipeline
        - if there is a next cmd
        - redirect the stdout to the write end of the pipe
        - close both end of the pipe
    - handle output redirect
    - return 1 on error, 0 on success
*/
int	setup_pipe_output(t_cmd *cmd, t_pipe *ctx, int *stdout_backup, t_env *env)
{
	if (cmd->next)
	{
		dup2(ctx->pipefd[1], STDOUT_FILENO);
		close(ctx->pipefd[0]);
		close(ctx->pipefd[1]);
	}
	if (cmd->outfile)
	{
		if (handle_output_redirect(cmd, stdout_backup, env) == -1)
			return (1);
	}
	return (0);
}
