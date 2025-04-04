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
#include "../../incl/minishell.h"

/* helper function for execute_pipe_child */
static void	cleanup_input_output(t_cmd *cmd_head,
		t_cmd *current, t_env *env, t_fd *fd)
{
	(void)current;
	if (fd->stdin != -1)
		close(fd->stdin);
	if (fd->stdout != -1)
		close(fd->stdout);
	free_cmds(cmd_head);
	free_env(env);
	exit(env->exit_status);
}

/* helper function for execute_pipe_child */
static void	cleanup_and_exit(t_cmd *cmd_head,
	t_cmd *current, t_env *env, t_fd *fd)
{
	(void)current;
	if (fd->stdin != -1)
		close(fd->stdin);
	if (fd->stdout != -1)
		close(fd->stdout);
	free_cmds(cmd_head);
	free_env(env);
	exit(env->exit_status);
}

/* helper function for execute_pipe_child
	to cleanup fd from previous process
*/
void	close_unused_fd(t_pipe *ctx)
{
	if (ctx->stdin_backup != -1)
		close(ctx->stdin_backup);
	if (ctx->stdout_backup != -1)
		close(ctx->stdout_backup);
	if (ctx->stderr_backup != -1)
		close(ctx->stderr_backup);
}

/* execute the child process in pipe */
void	execute_pipe_child(t_cmd *cmd_head, t_cmd *current,
		t_pipe *ctx, t_env *env)
{
	t_fd	fd_child;
	int		redirect_error;

	fd_child.stdin = -1;
	fd_child.stdout = -1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
	close_unused_fd(ctx);
	redirect_error = process_redirect(current, env);
	handle_redirect_error(current, ctx, redirect_error, env);
	if (setup_pipe_input(current, ctx, &fd_child.stdin, env))
		cleanup_input_output(cmd_head, current, env, &fd_child);
	if (setup_pipe_output(current, ctx, &fd_child.stdout, env))
		cleanup_input_output(cmd_head, current, env, &fd_child);
	if (current->args && current->args[0]
		&& ft_strcmp(current->args[0], "exit") == 0)
	{
		ft_exit(current->args, env);
		cleanup_and_exit(cmd_head, current, env, &fd_child);
	}
	execute_cmd(cmd_head, current, env, &fd_child);
	cleanup_and_exit(cmd_head, current, env, &fd_child);
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
