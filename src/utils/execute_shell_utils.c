/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_shell_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:01:10 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 11:55:27 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	execute_builtin(t_cmd *cmd, t_env *env)
{
	t_builtin	*builtins;
	int			i;

	printf("[DEBUG] execute_builtin called for: %s\n", cmd->args[0]);
	builtins = init_builtin();
	i = 0;
	while (builtins[i].builtin_name)
	{
		if (cmd->args && ft_strcmp(cmd->args[0], builtins[i].builtin_name) == 0)
		{
			printf("[DEBUG] Found matching builtin: %s\n", builtins[i].builtin_name);
			builtins[i].func(cmd->args, env);
			printf("[DEBUG] Builtin command completed\n");
			return ;
		}
		i++;
	}
	printf("[DEBUG] No matching builtin found\n");
}

void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;

	printf("[DEBUG] execute_external called for: %s\n", cmd->args[0]);
	path = get_cmd_path(cmd);
	if (path)
	{
		printf("[DEBUG] Found path for external command: %s\n", path);
		ft_execve(path, cmd->args, env);
		printf("[DEBUG] Freeing command path\n");
		free(path);
	}
	if (cmd->args && cmd->args[0])
	{
		printf("[DEBUG] Command not found\n");
		ft_putstr_fd(" command not found\n", 2);
	}
	env->exit_status = 127;
	return ;
}

int is_builtin(char *cmd)
{
	char	*builtins[8] = {"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL};
	int		i;

	printf("[DEBUG] Checking if %s is a builtin\n", cmd);
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
		{
			printf("[DEBUG] Found builtin: %s\n", cmd);
			return (1);
		}
		i++;
	}
	printf("[DEBUG] Not a builtin: %s\n", cmd);
	return (0);
}

void	execute_cmd(t_cmd *cmd, t_env *env)
{
	printf("[DEBUG] execute_cmd called for: %s\n", cmd->args[0]);
	
	if (is_builtin(cmd->args[0]))
	{
		printf("[DEBUG] Executing builtin command\n");
		execute_builtin(cmd, env);
		printf("[DEBUG] Builtin command execution completed\n");
		return ;
	}
	printf("[DEBUG] Executing external command\n");
	execute_external(cmd, env);
	printf("[DEBUG] External command execution completed\n");
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
