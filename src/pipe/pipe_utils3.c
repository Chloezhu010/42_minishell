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

	// printf("[DEBUG] Starting execute_pipe_child for command: %s\n", current->args[0]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	stdin_backup_child = -1;
	stdout_backup_child = -1;
	handle_redirect_error(current, ctx, redirect_error);
	handle_stderr_redirect(current);
	if (setup_pipe_input(current, ctx, &stdin_backup_child, env))
	{
		// printf("[DEBUG] Pipe input setup failed\n");
		if (stdin_backup_child != -1)
			close(stdin_backup_child);
		if (stdout_backup_child != -1)
			close(stdout_backup_child);
		exit(1);
	}
	if (setup_pipe_output(current, ctx, &stdout_backup_child, env))
	{
		// printf("[DEBUG] Pipe output setup failed\n");
		if (stdin_backup_child != -1)
			close(stdin_backup_child);
		if (stdout_backup_child != -1)
			close(stdout_backup_child);
		exit(1);
	}
	// printf("[DEBUG] About to execute command: %s\n", current->args[0]);
	execute_cmd(current, env);
	// printf("[DEBUG] Command executed, exiting with status: %d\n", env->exit_status);

	/* add free_env, free_cmds before exit */
	if (current->args[0] && ((ft_strcmp(current->args[0], "env") == 0
			|| ft_strcmp(current->args[0], "echo") == 0
			|| ft_strcmp(current->args[0], "cat") == 0
			|| ft_strcmp(current->args[0], "unset") == 0
			|| ft_strcmp(current->args[0], "export") == 0
			|| ft_strcmp(current->args[0], "unset") == 0
			|| ft_strcmp(current->args[0], "cd") == 0
			|| ft_strcmp(current->args[0], "pwd") == 0)))
		free_env(env);
	if (current)
		free_cmds(current);

	/* cleanup stdin & stdout backup */
	if (stdin_backup_child != -1)
		close(stdin_backup_child);
	if (stdout_backup_child != -1)
		close(stdout_backup_child);
	exit(env->exit_status);
}

/* handle parent process after fork
    - close prev pipe read end
    - if not the last cmd, prepare next pipe read end
    - track last cmd's pid
*/
void	execute_parent_process(t_pipe *ctx, t_cmd *cmd, pid_t pid)
{
	// printf("[DEBUG] Parent process handling for pid: %d\n", pid);
	ctx->pids[ctx->pid_count++] = pid;
	if (ctx->prev_pipe_read != -1)
	{
		// printf("[DEBUG] Closing prev_pipe_read in parent\n");
		close(ctx->prev_pipe_read);
		ctx->prev_pipe_read = -1;
	}
	if (cmd->next)
	{
		// printf("[DEBUG] Setting up next pipe read end\n");
		close(ctx->pipefd[1]);
		ctx->prev_pipe_read = ctx->pipefd[0];
	}
	if (!cmd->next)
	{
		// printf("[DEBUG] Setting last_pid: %d\n", pid);
		ctx->last_pid = pid;
	}
}

/* restore std fd */
void	restore_std_fd(t_pipe *ctx)
{
	// printf("[DEBUG] Restoring standard file descriptors\n");
	dup2(ctx->stdin_backup, STDIN_FILENO);
	dup2(ctx->stdout_backup, STDOUT_FILENO);
	dup2(ctx->stderr_backup, STDERR_FILENO);
	close(ctx->stdin_backup);
	close(ctx->stdout_backup);
	close(ctx->stderr_backup);
	// printf("[DEBUG] Standard file descriptors restored\n");
}

/* wait for all child process and set exit status */
void	wait_for_child(t_pipe *ctx, t_env *env)
{
	int	i;
	int	status;
	int	exit_code;

	// printf("[DEBUG] Waiting for child processes\n");
	i = 0;
	while (i < ctx->pid_count)
	{
		// printf("[DEBUG] Waiting for pid: %d\n", ctx->pids[i]);
		waitpid(ctx->pids[i], &status, 0);
		if (ctx->pids[i] == ctx->last_pid)
		{
			if ((status & 0x7F) == 0)
			{
				exit_code = (status >> 8) & 0xFF;
				// printf("[DEBUG] Last child exited with code: %d\n", exit_code);
				if (exit_code == 2)
					env->exit_status = 1;
				else
					env->exit_status = exit_code;
			}
		}
		i++;
	}
	// printf("[DEBUG] All child processes completed\n");
}
