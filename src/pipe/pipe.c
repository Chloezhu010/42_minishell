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

/* main pipeline execution
    - init pipeline ctx and mark cmds
    - check redirect before creating pipes
    - create pipe if not the last cmd
    - fork process
    - restore std fd
    - wait for all child process
*/
static void	fork_and_execute_pipe(t_cmd *current,
	t_pipe *ctx, t_env *env, int redirect_error)
{
	pid_t	pid;

	redirect_error = process_redirect(current, env);
	if (current->next)
	{
		if (create_cmd_pipe(ctx, env))
			return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit_status(env, 1);
		close_all_pipe_fds(ctx);
		restore_std_fd(ctx);
		return ;
	}
	if (pid == CHILD_PROCESS)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_pipe_child(current, ctx, env, redirect_error);
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

void	execute_pipeline(t_cmd *cmd, t_env *env)
{
	t_cmd				*current;
	t_pipe				ctx;
	int					redirect_error;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	init_pipe(&ctx);
	mark_pipeline_cmd(cmd);
	redirect_error = 0;
	current = cmd;
	save_signal_handlers(&old_int, &old_quit);
	set_parent_signals_for_pipeline();
	env->at_prompt = 0;

	printf("[DEBUG] execute_pipeline: cmd = %p\n", (void*)cmd);
	while (current)
	{
		// chec if current cmd is "exit"
		if (current->args && current->args[0] && ft_strcmp(current->args[0], "exit") == 0)
		{
			// if exit is the last cmd, directly execute exit
			if (!current->next)
			{
				ft_exit(current->args, env);
				break ;
			}
			// if exit in the middle of pipe, set flag and break
			env->exit_requested = 1;
			break ;
		}
		
		fork_and_execute_pipe(current, &ctx, env, redirect_error);
		// check if exit requested during pipe execution
		if (env->exit_requested)
		{
			// printf("[db] execute_pipeline: exit_requested %d\n, cleanup cmd", env->exit_requested);
			close_all_pipe_fds(&ctx);
			restore_std_fd(&ctx);
			free_cmds(cmd); //add cleanup
			break ;
		}
		current = current->next;
	}
	close_all_pipe_fds(&ctx);
	restore_std_fd(&ctx);
	// only wait for child if haven't requrested an exit
	if (!env->exit_requested)
	{
		wait_for_child(&ctx, env);
		// free_cmds(cmd);// add
	}
	// printf("[db] execute_pipeline: exit_requested %d\n", env->exit_requested);//debug
	reset_input_state();
	restore_signal_handlers(&old_int, &old_quit);
	env->at_prompt = 1;
}
