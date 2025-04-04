/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:01:41 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 12:13:36 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"

/* helper function for fork_and_execute_pipe */
static void	handle_fork_error(t_pipe *ctx, t_env *env)
{
	perror("fork");
	exit_status(env, 1);
	close_all_pipe_fds(ctx);
	restore_std_fd(ctx);
}

/* main pipeline execution
    - init pipeline ctx and mark cmds
    - check redirect before creating pipes
    - create pipe if not the last cmd
    - fork process
    - restore std fd
    - wait for all child process
*/
static void	fork_and_execute_pipe(t_cmd *cmd_head, t_cmd *current,
		t_pipe *ctx, t_env *env)
{
	pid_t	pid;
	int		redirect_error;

	redirect_error = process_redirect(current, env);
	if (current->next)
	{
		if (create_cmd_pipe(ctx, env))
			return ;
	}
	pid = fork();
	if (pid == -1)
	{
		handle_fork_error(ctx, env);
		return ;
	}
	if (pid == CHILD_PROCESS)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_pipe_child(cmd_head, current, ctx, env);
	}
	else
		execute_parent_process(ctx, current, pid);
}

/* Save the original signal handlers before changing them */
static void	save_signal_handlers(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
}

/* Set parent process signal handling during pipeline execution */
static void	set_parent_signals_for_pipeline(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/* Restore original signal handlers */
static void	restore_signal_handlers(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

/* helper function for execute_pipeline
	- setup pipeline
*/
static void	setup_pipeline(t_pipe *ctx, t_cmd *cmd,
	t_env *env, struct sigaction old[2])
{
	init_pipe(ctx);
	mark_pipeline_cmd(cmd);
	save_signal_handlers(&old[0], &old[1]);
	set_parent_signals_for_pipeline();
	env->at_prompt = 0;
}

/* helper function for execute_pipeline
	- cleanup pipeline
*/
static void	cleanup_pipeline(t_pipe *ctx, t_cmd *cmd, 
	t_env *env,	struct sigaction old[2])
{
	close_all_pipe_fds(ctx);
	restore_std_fd(ctx);
	if (!env->exit_requested)
	{
		wait_for_child(ctx, env);
		free_cmds(cmd);
	}
	else
		free_cmds(cmd);
	reset_input_state();
	restore_signal_handlers(&old[0], &old[1]);
	env->at_prompt = 1;
}

/* helper function for process_pipeline
	- handle exit_requested case
*/
// static void	is_exit_request(t_env *env)


/* helper function for execute_pipeline
	- process pipeline
*/
static void	process_pipeline(t_pipe *ctx, t_cmd *cmd, t_env *env)
{
	t_cmd	*current;
	int		redirect_error;

	redirect_error = 0;
	current = cmd;
	while (current)
	{
		if (current->args && current->args[0]
			&& ft_strcmp(current->args[0], "exit") == 0)
		{
			if (!current->next)
			{
				ft_exit(current->args, env);
				break ;
			}
			env->exit_requested = 1;
			break ;
		}
		fork_and_execute_pipe(cmd, current, ctx, env);
		if (env->exit_requested)
		{
			close_all_pipe_fds(ctx);
			restore_std_fd(ctx);
			free_cmds(cmd);
			break ;
		}
		current = current->next;
	}
}

void	execute_pipeline(t_cmd *cmd, t_env *env)
{
	t_cmd				*current;
	t_pipe				ctx;
	struct sigaction	old[2];
	int					redirect_error;

	redirect_error = 0;
	current = cmd;
	setup_pipeline(&ctx, cmd, env, old);
	process_pipeline(&ctx, cmd, env);
	cleanup_pipeline(&ctx, cmd, env, old);
}

/* original function */
// void	execute_pipeline(t_cmd *cmd, t_env *env)
// {
// 	t_cmd				*current;
// 	t_pipe				ctx;
// 	int					redirect_error;
// 	struct sigaction	old_int;
// 	struct sigaction	old_quit;

// 	init_pipe(&ctx);
// 	mark_pipeline_cmd(cmd);
// 	redirect_error = 0;
// 	current = cmd;
// 	save_signal_handlers(&old_int, &old_quit);
// 	set_parent_signals_for_pipeline();
// 	env->at_prompt = 0;
// 	while (current)
// 	{
// 		if (current->args && current->args[0]
// 			&& ft_strcmp(current->args[0], "exit") == 0)
// 		{
// 			if (!current->next)
// 			{
// 				ft_exit(current->args, env);
// 				break ;
// 			}
// 			env->exit_requested = 1;
// 			break ;
// 		}
// 		fork_and_execute_pipe(cmd, current, &ctx, env);
// 		if (env->exit_requested)
// 		{
// 			close_all_pipe_fds(&ctx);
// 			restore_std_fd(&ctx);
// 			free_cmds(cmd);
// 			break ;
// 		}
// 		current = current->next;
// 	}
// 	close_all_pipe_fds(&ctx);
// 	restore_std_fd(&ctx);
// 	if (!env->exit_requested)
// 	{
// 		wait_for_child(&ctx, env);
// 		free_cmds(cmd);
// 	}
// 	else
// 	{
// 		free_cmds(cmd);
// 	}
// 	reset_input_state();
// 	restore_signal_handlers(&old_int, &old_quit);
// 	env->at_prompt = 1;
// }
