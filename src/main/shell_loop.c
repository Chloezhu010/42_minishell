#include "../../incl/minishell.h"

/* global variables */
int g_exit_status = 0;

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
        full_path = find_path(args[0]);
        if (!full_path)
        {
            perror("Command not found");
            exit(127);
        }
        ft_execve(full_path, args, env);
    }
    else
        ft_wait(&status);
}

/* execute shell
    - input control
    - check env init
    - if it's build in function, call it
    - if not, launch external programs
*/
void execute_shell(char **args, t_env *env)
{
    int i;
    t_builtin *builtin_in;

    if (!args[0])
        return ;
    if (!env)
    {
        printf("env not initialized\n");
        return ;
    }
    builtin_in = init_builtin();
    i = 0;
    while (builtin_in[i].builtin_name)
    {
        if(ft_strcmp(args[0], builtin_in[i].builtin_name) == 0)
        {
            builtin_in[i].func(args, env);
            return ;
        }
        i++;
    }
    launch_execution(args, env);
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
    char **args;
	t_token *tokens;
	t_cmd	*cmds;

    setup_signal();
    while (1)
    {
        // 2. read line from command
        line = read_line();
        if (line == NULL)
        {
            printf("exit\n");
            break;            
        }
        // add non-empty line to history
        if (*line)
            add_history(line);
        // 3. parse the args
		tokens = tokenize(line);
		if (tokens != NULL)
		{
			expand_tokens(tokens);
			check_format_command(tokens);
			// print_tokens(tokens);
			cmds = parse_tokens(tokens);
			// if (cmds)
			// 	print_cmds(cmds);
			free_tokens(tokens);
		}
		if (cmds != NULL)
		{
        	args = cell_split_line(line);
        // 4. execute the command
        	execute_shell(args, env);
			if (!args)
                ft_freeup(args);
            if (!cmds)
			    free_cmds(cmds);
		}
        // 5. cleanup before exit
            if (!line)
        	    free(line);
            
    }
}

int main(int ac, char **av, char **envp)
{    
    t_env env;
    
    (void)ac;
    (void)av;
    init_env(&env, envp);
    shell_loop(&env);
    free_env(&env);
    enable_echo();
    rl_clear_history(); //linux change to: rl_clear_history()
    return (g_exit_status);
}