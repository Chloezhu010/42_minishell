/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:58:16 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 17:09:45 by auzou            ###   ########.fr       */
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
		execute_cmd(cmd, env);
	else
	{
		ft_wait(&status);
		if (WIFEXITED(status))
			env->exit_status = WEXITSTATUS(status);
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
	expand_tokens(tokens, env);
	check_format_command(tokens);
	return (tokens);
}

/* shell loop
    - setup signal handler
    - read line from the command
        - add non-empty line to history
    - parse the args
    - execute the args
    - cleanup before exit
*/
void	shell_loop(t_env *env)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	setup_signal(env);
	while (1)
	{
		env->at_prompt = 1; //add
		line = get_command_line();
		env->at_prompt = 0; //add
		if (line == NULL)
			break ;
		tokens = process_command_line(line, env);
		if (tokens)
		{
			cmds = parse_tokens(tokens);
			if (cmds)
			{
				process_heredocs(cmds, env);
				execute_commands(cmds, env);
				free_cmds(cmds);
			}
			free_tokens(tokens);
		}
		free(line);
		if (env->exit_requested)
			break ;
	}
}
