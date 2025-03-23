/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:01:41 by auzou             #+#    #+#             */
/*   Updated: 2025/03/23 14:20:56 by czhu             ###   ########.fr       */
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
		env->exit_status = 1;
		return ;
	}
	if (pid == CHILD_PROCESS)
		execute_pipe_child(current, ctx, env, redirect_error);
	else
		execute_parent_process(ctx, current, pid);
}

void	execute_pipeline(t_cmd *cmd, t_env *env)
{
	t_cmd	*current;
	t_pipe	ctx;
	int		redirect_error;

	init_pipe(&ctx);
	mark_pipeline_cmd(cmd);
	redirect_error = 0;
	current = cmd;
	while (current)
	{
		fork_and_execute_pipe(current, &ctx, env, redirect_error);
		current = current->next;
	}
	restore_std_fd(&ctx);
	wait_for_child(&ctx, env);
}
