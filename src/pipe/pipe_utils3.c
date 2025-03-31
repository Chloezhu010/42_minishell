/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 12:25:48 by czhu              #+#    #+#             */
/*   Updated: 2025/03/31 19:47:28 by auzou            ###   ########.fr       */
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

/* wait for all child process and set exit status */
void	wait_for_child(t_pipe *ctx, t_env *env)
{
	int	i;
	int	status;
	int	exit_code;

	i = 0;
	while (i < ctx->pid_count)
	{
		waitpid(ctx->pids[i], &status, 0);
		if (ctx->pids[i] == ctx->last_pid)
		{
			if ((status & 0x7F) == 0)
			{
				exit_code = (status >> 8) & 0xFF;
				if (exit_code == 2)
					exit_status(env, 1);
				else
					exit_status(env, exit_code);
			}
		}
		i++;
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
