/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:14:44 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:20:53 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* execute the child process in pipe */
void	execute_pipe_child(t_cmd *current,
	t_pipe *ctx, t_env *env, int redirect_error)
{
	int	stdin_backup_child;
	int	stdout_backup_child;

	stdin_backup_child = -1;
	stdout_backup_child = -1;
	// printf("DEBUG: Child - cmd: %s, setting up stdin/stdout\n", current->args[0]);

	handle_redirect_error(current, ctx, redirect_error);
	handle_stderr_redirect(current);
	if (setup_pipe_input(current, ctx, &stdin_backup_child, env))
		exit(1);
	if (setup_pipe_output(current, ctx, &stdout_backup_child, env))
		exit(1);

	// printf("DEBUG: Child - before execute_cmd: %s\n", current->args[0]); 
	execute_cmd(current, env);
	// printf("DEBUG: Child - after execute_cmd: %s\n", current->args[0]);
	
	// restore_io(stdin_backup_child, stdout_backup_child);
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

		// printf("DEBUG: Parent - prev_pipe_read set to %d\n", ctx->prev_pipe_read);
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
		if (ctx->pids[i] == ctx->last_pid && WIFEXITED(status))
		{
			exit_code = WEXITSTATUS(status);
			if (exit_code == 2)
				env->exit_status = 1;
			else
				env->exit_status = exit_code;
		}
		i++;
	}
}
