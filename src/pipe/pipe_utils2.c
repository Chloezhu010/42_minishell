/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chloe <chloe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:14:04 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 23:10:44 by chloe            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* handle redirect error
    - if there is a next cmd
        - close the read & write end of the pipe
    - if there is a prev_pipe_read, close it too
	- free cmds and env
    - exit 1
*/
void	handle_redirect_error(t_cmd *cmd, t_pipe *ctx,
		int redirect_error, t_env *env)
{
	if (redirect_error)
	{
		if (cmd->next)
		{
			close(ctx->pipefd[0]);
			close(ctx->pipefd[1]);
		}
		if (ctx->prev_pipe_read != -1)
		{
			close(ctx->prev_pipe_read);
			ctx->prev_pipe_read = -1;
		}
		free_cmds(cmd);
		free_env(env);
		exit(1);
	}
}

/* helper function for setup_pipe_input */
static int	cleanup_error(t_cmd *cmd, t_pipe *ctx, int *stdin_backup)
{
	if (*stdin_backup != -1)
		close(*stdin_backup);
	if (cmd->next)
	{
		close(ctx->pipefd[0]);
		close(ctx->pipefd[1]);
	}
	if (ctx->prev_pipe_read != -1)
	{
		close(ctx->prev_pipe_read);
		ctx->prev_pipe_read = -1;
	}
	return (1);
}

/* helper function for setup_pipe_input */
static int	handle_pipe_dup_error(t_cmd *cmd, t_pipe *ctx)
{
	close(ctx->prev_pipe_read);
	ctx->prev_pipe_read = -1;
	if (cmd->next)
	{
		close(ctx->pipefd[0]);
		close(ctx->pipefd[1]);
	}
	return (1);
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
			cleanup_error(cmd, ctx, stdin_backup);
	}
	else if (ctx->prev_pipe_read != -1)
	{
		if (ft_dup(STDIN_FILENO, stdin_backup, "dup"))
			handle_pipe_dup_error(cmd, ctx);
		if (ft_dup2(ctx->prev_pipe_read, STDIN_FILENO, *stdin_backup, "dup2"))
		{
			close(*stdin_backup);
			*stdin_backup = -1;
			handle_pipe_dup_error(cmd, ctx);
		}
		close(ctx->prev_pipe_read);
		ctx->prev_pipe_read = -1;
	}
	return (0);
}
