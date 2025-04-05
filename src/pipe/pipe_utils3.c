/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 12:25:48 by czhu              #+#    #+#             */
/*   Updated: 2025/04/02 11:57:48 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* helper function for setup_pipe_output */
static int	handle_dup2_error(t_pipe *ctx, int *stdout_backup)
{
	if (*stdout_backup != -1)
		close(*stdout_backup);
	close(ctx->pipefd[0]);
	close(ctx->pipefd[1]);
	return (1);
}

/* helper function for setup_pipe_output */
static int	handle_output_redirect_error(int *stdout_backup,
	t_cmd *cmd, t_pipe *ctx)
{
	if (*stdout_backup != -1)
	{
		close(*stdout_backup);
		*stdout_backup = -1;
	}	
	if (cmd->next && ctx->pipefd[1] != -1)
	{
		close(ctx->pipefd[1]);
		ctx->pipefd[1] = -1;
	}
	return (1);
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
		if (ft_dup(STDOUT_FILENO, stdout_backup, "dup"))
		{
			close(ctx->pipefd[0]);
			close(ctx->pipefd[1]);
			return (1);
		}
		if (ft_dup2(ctx->pipefd[1], STDOUT_FILENO, *stdout_backup, "dup2"))
			handle_dup2_error(ctx, stdout_backup);
		close(ctx->pipefd[0]);
		close(ctx->pipefd[1]);
	}
	if (cmd->outfile)
	{
		if (handle_output_redirect(cmd, stdout_backup, env) == -1)
			handle_output_redirect_error(stdout_backup, cmd, ctx);
	}
	return (0);
}

/* restore std fd */
void	restore_std_fd(t_pipe *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		dup2(ctx->stdin_backup, STDIN_FILENO);
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		dup2(ctx->stdout_backup, STDOUT_FILENO);
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
	if (ctx->stderr_backup != -1)
	{
		dup2(ctx->stderr_backup, STDERR_FILENO);
		close(ctx->stderr_backup);
		ctx->stderr_backup = -1;
	}
}

/* Helper function to clean up all pipe file descriptors */
void	close_all_pipe_fds(t_pipe *ctx)
{
	if (ctx->prev_pipe_read != -1)
	{
		close(ctx->prev_pipe_read);
		ctx->prev_pipe_read = -1;
	}
	if (ctx->pipefd[0] != -1)
	{
		close(ctx->pipefd[0]);
		ctx->pipefd[0] = -1;
	}
	if (ctx->pipefd[1] != -1)
	{
		close(ctx->pipefd[1]);
		ctx->pipefd[1] = -1;
	}
}
