#include "../../incl/minishell.h"

/* read the input from user
    - print the cwd before $>
    - read the input
*/
char *read_line(void)
{
    char *buf;
    // char cwd[1024];

    buf = NULL;
    // if (!getcwd(cwd, sizeof(cwd)))
    //     perror("getcwd");
    // printf("%s ", cwd);
	buf = readline("minishell $>");
    return (buf);
}

/* tokenize the input line
    - parse the args
*/
char    **cell_split_line(char *line)
{
    char            **tokens;
    
    if (!line)
        return (NULL);
    tokens = ft_split(line, ' ');
    return (tokens);
}

/* launch external programs */
void launch_execution(char **args, t_env *env)
{
    pid_t pid;
    int status = 0;
    char *full_path;

    pid = ft_fork();
    if (pid == CHILD_PROCESS)
    {
        if (!args[0])
            exit(1);
        if (ft_strchr(args[0], '/'))
            full_path = ft_strdup(args[0]);
        else
            full_path = find_path(args[0]);
        if (!full_path)
        {
            ft_putstr_fd(" command not found\n", 2);
            exit(127);
        }
        ft_execve(full_path, args, env);
    }
    else
    {
        ft_wait(&status);
        if (WIFEXITED(status))
            env->exit_status = WEXITSTATUS(status);
    }
}

/* execute shell
    - input control
    - check env init
    - handle input redirect for heredoc
    - if it's build in function, call it
    - if not, launch external programs
*/
void execute_shell(t_cmd *cmd, t_env *env)
{
    int i;
    int stdin_backup;
    int stdout_backup;
    t_builtin *builtin_in;
    // int input_error = 0;

    stdin_backup = -1;
    stdout_backup = -1;
    if (!cmd->args[0] || !env)
        return ;
    
    if (!cmd->in_pipe && process_redirect(cmd, env))
        return ;

    // /* check input file first */
    // input_error = check_input_file(cmd, env);
    // /* only create output file */
    // if (create_output_file(cmd, env))
    //     exit(1);
    // /* if there is input error, return */
    // if (input_error)
    //     return ;


    /* handle redirections */
    if (handle_input_redirect(cmd, &stdin_backup, env) == -1
        || handle_output_redirect(cmd, &stdout_backup, env) == -1)
    {
        env->exit_status = 1;
        restore_io(stdin_backup, stdout_backup);
        return ;
    }

    /* handle builtin & external cmd execution */
    builtin_in = init_builtin();
    i = 0;
    while (builtin_in[i].builtin_name)
    {
        if(ft_strcmp(cmd->args[0], builtin_in[i].builtin_name) == 0)
        {
            builtin_in[i].func(cmd->args, env);
            /* restore original stdin after the execution of builtin */
            restore_io(stdin_backup, stdout_backup);
            return ;
        }
        i++;
    }
    if (!builtin_in[i].builtin_name)
        launch_execution(cmd->args, env);
    /* restore original stdin after the execution of eternal program */
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