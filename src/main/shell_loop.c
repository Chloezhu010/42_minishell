/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:58:16 by auzou             #+#    #+#             */
/*   Updated: 2025/03/26 23:25:31 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

char	*read_line(void)
{
	char	*buf;

	buf = NULL;
	buf = readline("minishell $>");
	return (buf);
}

char	**cell_split_line(char *line)
{
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = ft_split(line, ' ');
	return (tokens);
}

void	execute_shell(t_cmd *cmd, t_env *env)
{
	int		stdin_backup;
	int		stdout_backup;
	int		status;
	pid_t	pid;

	stdin_backup = -1;
	stdout_backup = -1;
	if (!cmd->args[0] || !env)
		return ;
	if (handle_redirect(cmd, &stdin_backup, &stdout_backup, env))
		return ;
	if (execute_builtin1(cmd, env, stdin_backup, stdout_backup))
		return ;
	pid = ft_fork();
	if (pid == CHILD_PROCESS)
	{
		execute_cmd(cmd, cmd, env);
		// printf("[DEBUG] After execute_cmds: cmd = %p\n", (void*)cmd);
	}
	else
	{
		ft_wait(&status);
		if ((status & 0x7F) == 0)
			exit_status(env, (status >> 8) & 0xFF);
	}
	restore_io(stdin_backup, stdout_backup);
}

t_token	*process_command_line(char *line, t_env *env)
{
	t_token	*tokens;

	if (!line || !*line)
		return (NULL);
	add_history(line);
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	print_tokens(tokens);
	expand_tokens(tokens, env);
	merge_consecutive_tokens(tokens);
	check_format_command(tokens);
	return (tokens);
}

void	process_tokens_and_execute(t_token *tokens, t_env *env)
{
	t_cmd	*cmds;

	cmds = parse_tokens(tokens);
	if (!cmds)
		exit_status(env, 2);
	free_tokens(tokens);
	if (cmds)
	{
		int pipeline_executed;

		process_heredocs(cmds, env);
		pipeline_executed = execute_commands(cmds, env);
		if (!pipeline_executed)
			free_cmds(cmds);
	}
}
