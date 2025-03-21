/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_shell_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:01:10 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:01:11 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	execute_builtin(t_cmd *cmd, t_env *env)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtin();
	i = 0;
	while (builtins[i].builtin_name)
	{
		if (cmd->args && ft_strcmp(cmd->args[0], builtins[i].builtin_name) == 0)
		{
			builtins[i].func(cmd->args, env);
			exit(env->exit_status);
		}
		i++;
	}
}

void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;

	path = get_cmd_path(cmd);
	if (path)
	{
		ft_execve(path, cmd->args, env);
		free(path);
	}
	if (cmd->args && cmd->args[0])
		perror("command not found");
	exit(127);
}

void	execute_cmd(t_cmd *cmd, t_env *env)
{
	execute_builtin(cmd, env);
	execute_external(cmd, env);
}

int	handle_redirect(t_cmd *cmd, int *stdin_backup,
	int *stdout_backup, t_env *env)
{
	if (!cmd->in_pipe && process_redirect(cmd, env))
		return (1);
	if (handle_input_redirect(cmd, stdin_backup, env) == -1
		|| handle_output_redirect(cmd, stdout_backup, env) == -1)
	{
		env->exit_status = 1;
		return (1);
	}
	return (0);
}

int	execute_builtin1(t_cmd *cmd, t_env *env,
	int stdin_backup, int stdout_backup)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtin();
	i = 0;
	while (builtins[i].builtin_name)
	{
		if (ft_strcmp(cmd->args[0], builtins[i].builtin_name) == 0)
		{
			builtins[i].func(cmd->args, env);
			restore_io(stdin_backup, stdout_backup);
			return (1);
		}
		i++;
	}
	return (0);
}
