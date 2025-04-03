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

/* helper function for execute_pipe_child */
static void	cleanup_input_output(t_cmd *cmd_head, t_cmd *current, t_env *env,
	int stdin_backup_child, int stdout_backup_child)
{
	(void)current; // Suppress unused parameter warning if needed
	if (stdin_backup_child != -1)
		close(stdin_backup_child);
	if (stdout_backup_child != -1)
		close(stdout_backup_child);
	free_cmds(cmd_head); // Free the entire command list copy
	free_env(env);
	exit(env->exit_status);
}

// /* helper function for execute_pipe_child */
// static int	is_builtin_command(char *cmd_name)
// {
// 	return (cmd_name && (
// 			ft_strcmp(cmd_name, "env") == 0
// 			|| ft_strcmp(cmd_name, "exit") == 0
// 			|| ft_strcmp(cmd_name, "echo") == 0
// 			|| ft_strcmp(cmd_name, "cat") == 0
// 			|| ft_strcmp(cmd_name, "unset") == 0
// 			|| ft_strcmp(cmd_name, "export") == 0
// 			|| ft_strcmp(cmd_name, "unset") == 0
// 			|| ft_strcmp(cmd_name, "cd") == 0
// 			|| ft_strcmp(cmd_name, "pwd") == 0
// 			|| ft_strcmp(cmd_name, "grep") == 0));
// }

/* helper function for execute_pipe_child */
static void	cleanup_and_exit(t_cmd *cmd_head, t_cmd *current, t_env *env,
		int stdin_backup_child, int stdout_backup_child)
{
	// if (current->args[0] && is_builtin_command(current->args[0]))
	// 	free_env(env);
	// if (current)
	// 	free_cmds(current);
	(void)current; // Suppress unused parameter warning if needed
	if (stdin_backup_child != -1)
		close(stdin_backup_child);
	if (stdout_backup_child != -1)
		close(stdout_backup_child);
	free_cmds(cmd_head); // Free the entire command list copy
	free_env(env);// add
	exit(env->exit_status);
}

/* execute the child process in pipe */
void	execute_pipe_child(t_cmd *cmd_head, t_cmd *current,
	t_pipe *ctx, t_env *env, int redirect_error)
{
	int	stdin_backup_child;
	int	stdout_backup_child;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	stdin_backup_child = -1;
	stdout_backup_child = -1;
	handle_redirect_error(current, ctx, redirect_error, env);
	// Add debug print before setup_pipe_input if needed
	// printf("[DEBUG CHILD %d] Before setup_pipe_input\n", getpid());
	if (setup_pipe_input(current, ctx, &stdin_backup_child, env))
		cleanup_input_output(cmd_head, current, env,
			stdin_backup_child, stdout_backup_child);
	// Add debug print after setup_pipe_input if needed
	// printf("[DEBUG CHILD %d] After setup_pipe_input, stdin_backup_child=%d\n", getpid(), stdin_backup_child);
	if (setup_pipe_output(current, ctx, &stdout_backup_child, env))
		cleanup_input_output(cmd_head, current, env,
			stdin_backup_child, stdout_backup_child);
	// Add debug print after setup_pipe_output if needed
	// printf("[DEBUG CHILD %d] After setup_pipe_output, stdout_backup_child=%d\n", getpid(), stdout_backup_child);
	
	// special handling for exit cmd in pipe
	if (current->args && current->args[0] && ft_strcmp(current->args[0], "exit") == 0)
	{
		ft_exit(current->args, env);
		// //add cleanup
		// if (stdin_backup_child != -1)
        //     close(stdin_backup_child);
        // if (stdout_backup_child != -1)
        //     close(stdout_backup_child);
        // free_env(env);
        // exit(env->exit_status);
		// printf("[DEBUG CHILD %d] cleanup_and_exit from exit builtin\n", getpid());
		cleanup_and_exit(cmd_head, current, env, stdin_backup_child, stdout_backup_child);
	}

	execute_cmd(current, env);
	printf("[DEBUG] After execute_cmds: cmd = %p\n", (void*)current);
	// printf("[db] execute_pipe_child: exit_requested %d\n", env->exit_requested);
	// if (stdin_backup_child != -1)
    //         close(stdin_backup_child);
	// if (stdout_backup_child != -1)
	// 	close(stdout_backup_child);
	// free_env(env);
	// exit(env->exit_status);
	// printf("[DEBUG CHILD %d] cleanup_and_exit after execute_cmd\n", getpid());
	cleanup_and_exit(cmd_head, current, env, stdin_backup_child, stdout_backup_child);
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
