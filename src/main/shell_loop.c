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
	int		status;
	pid_t	pid;
	t_fd	fd;

	fd.stdin = -1;
	fd.stdout = -1;
	if (!cmd->args[0] || !env)
		return ;
	if (handle_redirect(cmd, &fd.stdin, &fd.stdout, env))
		return ;
	if (execute_builtin1(cmd, env, fd.stdin, fd.stdout))
		return ;
	pid = ft_fork();
	if (pid == CHILD_PROCESS)
		execute_cmd(cmd, cmd, env, &fd);
	else
	{
		ft_wait(&status);
		if ((status & 0x7F) == 0)
			exit_status(env, (status >> 8) & 0xFF);
	}
	restore_io(fd.stdin, fd.stdout);
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
	expand_tokens(tokens, env);
	merge_consecutive_tokens(tokens);
	check_format_command(tokens);
	return (tokens);
}

void	process_tokens_and_execute(t_token *tokens, t_env *env)
{
	t_cmd	*cmds;
	int		pipeline_executed;

	cmds = parse_tokens(tokens);
	if (!cmds)
		exit_status(env, 2);
	free_tokens(tokens);
	if (cmds)
	{
		env->heredoc_interrupted = 0;
		process_heredocs(cmds, env);
		if (!env->heredoc_interrupted)
		{
			pipeline_executed = execute_commands(cmds, env);
			if (!pipeline_executed)
				free_cmds(cmds);
		}
		else
			free_cmds(cmds);
	}
}
