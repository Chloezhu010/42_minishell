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

static int	execute_builtin1(t_cmd *cmd, t_env *env,
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

/* shell loop
    - setup signal handler
    - read line from the command
        - add non-empty line to history
    - parse the args
    - execute the args
    - cleanup before exit
*/

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

void	process_heredocs(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd_temp;
	int		fd;

	cmd_temp = cmds;
	while (cmd_temp)
	{
		if (cmd_temp->heredoc && cmd_temp->delimiter)
		{
			fd = handle_heredoc(cmd_temp->delimiter, env);
			if (fd != -1)
				cmd_temp->fd_in = fd;
		}
		cmd_temp = cmd_temp->next;
	}
}

void	execute_commands(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd_temp;

	if (!cmds)
		return ;
	if (cmds->next)
	{
		cmd_temp = cmds;
		while (cmd_temp)
		{
			cmd_temp->in_pipe = 1;
			cmd_temp = cmd_temp->next;
		}
		execute_pipeline(cmds, env);
	}
	else
	{
		cmds->in_pipe = 0;
		execute_shell(cmds, env);
	}
}

char	*get_command_line(void)
{
	char	*line;

	line = read_line();
	if (line == NULL)
	{
		printf("exit\n");
		return (NULL);
	}
	return (line);
}

void	shell_loop(t_env *env)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	setup_signal(env);
	while (1)
	{
		line = get_command_line();
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
	}
}

/* main function */
int main(int ac, char **av, char **envp)
{    
    t_env env;
    
    (void)ac;
    (void)av;
    init_env(&env, envp);
    shell_loop(&env);
    free_env(&env);
    enable_echo();
    rl_clear_history();
    return (env.exit_status);
}