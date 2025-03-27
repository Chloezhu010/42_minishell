/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:14:44 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 12:08:20 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* execute the child process in pipe
	- dont need to restore here
*/
void	execute_pipe_child(t_cmd *current,
	t_pipe *ctx, t_env *env, int redirect_error)
{
	int	stdin_backup_child;
	int	stdout_backup_child;

	signal(SIGINT, SIG_DFL); //adds
	signal(SIGQUIT, SIG_DFL); //add
	stdin_backup_child = -1;
	stdout_backup_child = -1;
	handle_redirect_error(current, ctx, redirect_error);
	handle_stderr_redirect(current);
	if (setup_pipe_input(current, ctx, &stdin_backup_child, env))
		exit(1);
	if (setup_pipe_output(current, ctx, &stdout_backup_child, env))
		exit(1);
	execute_cmd(current, env);
	exit(env->exit_status);
}

/* handle parent process after fork
    - close prev pipe read end
    - if not the last cmd, prepare next pipe read end
    - track last cmd's pid
*/
void	execute_parent_process(t_pipe *ctx, t_cmd *cmd, pid_t pid)
{
	ctx->pids[ctx->pid_count++] = pid;
	if (ctx->prev_pipe_read != -1)
	{
		close(ctx->prev_pipe_read);
		ctx->prev_pipe_read = -1;
	}
	if (cmd->next)
	{
		close(ctx->pipefd[1]);
		ctx->prev_pipe_read = ctx->pipefd[0];
	}
	if (!cmd->next)
		ctx->last_pid = pid;
}

/* restore std fd */
void	restore_std_fd(t_pipe *ctx)
{
	dup2(ctx->stdin_backup, STDIN_FILENO);
	dup2(ctx->stdout_backup, STDOUT_FILENO);
	dup2(ctx->stderr_backup, STDERR_FILENO);
	close(ctx->stdin_backup);
	close(ctx->stdout_backup);
	close(ctx->stderr_backup);
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
					env->exit_status = 1;
				else
					env->exit_status = exit_code;
			}
		}
		i++;
	}
}
