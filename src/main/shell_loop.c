#include "../../incl/minishell.h"

/* read the input from user
    - print the cwd before $>
    - read the input
*/
char *read_line(void)
{
    char *buf;

    buf = NULL;
	buf = readline("minishell $>");
    return (buf);
}

/* tokenize the input line */
char    **cell_split_line(char *line)
{
    char            **tokens;
    
    if (!line)
        return (NULL);
    tokens = ft_split(line, ' ');
    return (tokens);
}

/* execute a builtin cmd */
void execute_builtin(t_cmd *cmd, t_env *env)
{
    t_builtin *builtins;
    int i;

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

/* execute external functions
    - if the cmd name contains absolute path, then copy the path
        - if not, find_path
        - if path exists, execve the external function
    - if cmd failed
        - return error
*/
void execute_external(t_cmd *cmd, t_env *env)
{
    char *path;

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

/* execute both builtin and external functions */
void execute_cmd(t_cmd *cmd, t_env *env)
{
    execute_builtin(cmd, env);
    execute_external(cmd, env);
}

/* handle redirection */
int handle_redirect(t_cmd *cmd, int *stdin_backup, int *stdout_backup, t_env *env)
{
    if (!cmd->in_pipe && process_redirect(cmd, env))
        return (1);
    if (handle_input_redirect(cmd, stdin_backup, env) == -1
        || handle_output_redirect(cmd, stdout_backup, env) == -1)
    {
        env->exit_status = 1;
        // restore_io(stdin_backup, stdout_backup);
        return (1);
    }
    return (0);
}

// /* launch external programs */
// void launch_execution(t_cmd *cmd, t_env *env)
// {
//     pid_t pid;
//     int status;
//     // char *full_path;

//     pid = ft_fork();
//     if (pid == CHILD_PROCESS)
//     {
//         if (!cmd->args[0])
//             exit(1);
//         execute_external(cmd, env);
//     }
//     else
//     {
//         ft_wait(&status);
//         if (WIFEXITED(status))
//             env->exit_status = WEXITSTATUS(status);
//     }
// }

/* execute shell
    - input control
    - check env init
    - handle input redirect for heredoc
    - if it's build in function, call it
    - if not, launch external programs
*/
void execute_shell(t_cmd *cmd, t_env *env)
{
    int stdin_backup;
    int stdout_backup;
    pid_t pid;
    int status;

    stdin_backup = -1;
    stdout_backup = -1;
    if (!cmd->args[0] || !env)
        return ;
    if (handle_redirect(cmd, &stdin_backup, &stdout_backup, env))
        return ;
    /* if in a pipe, shouldn't call this function directly */
    if (cmd->in_pipe)
    {
        perror("execute_shell called directly for piped cmd");
        return ;
    }
    /* for singale cmd, fork and execute */
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
void shell_loop(t_env *env)
{
    char *line;
	t_token *tokens;
	t_cmd	*cmds;
    t_cmd   *cmd_temp;
    int     fd;

    setup_signal(env);
    while (1)
    {
        /* read line from command */
        line = read_line();
        if (line == NULL)
        {
            printf("exit\n");
            break;            
        }
        /* add non-empty line to history */
        if (*line)
            add_history(line);
        /* 3. parse the args */
		tokens = tokenize(line);
		if (tokens)
		{
			expand_tokens(tokens, env);
			check_format_command(tokens);
			cmds = parse_tokens(tokens);
            /* execute the cmds */
            if (cmds)
            {
                /* process all redirect before execution */
                cmd_temp = cmds;
                while (cmd_temp)
                {
                    /* handle heredoc */
                    if (cmd_temp->heredoc && cmd_temp->delimiter)
                    {
                        fd = handle_heredoc(cmd_temp->delimiter, env);
                        if (fd != -1)
                            cmd_temp->fd_in = fd;
                    }
                    cmd_temp = cmd_temp->next;
                }
                /*  then handle execution
                    - if there is muliple cmds, use execute_pipeline
                    - if only 1 cmd, use execute_shell
                    - free_cmds at the end
                */
                if (cmds && cmds->next)
                {
                    /* for pipes, mark all cmds */
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
                free_cmds(cmds);
            }
			free_tokens(tokens);
		}
        /* cleanup before exit */
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