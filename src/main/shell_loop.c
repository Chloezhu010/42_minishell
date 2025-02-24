#include "../incl/minishell.h"

/* global variables */
int status = 0;
t_builtin g_builtin[] = 
{
    {"pwd", ft_pwd},
    {"cd", ft_cd},
    {"echo", ft_echo},
    {"env", ft_env},
    {"exit", ft_exit},
    {"export", ft_export},
    {"unset", ft_unset},
    {NULL, NULL}
};

/* read the input from user
    - print the cwd before $>
    - read the input
*/
char *read_line(void)
{
    char *buf;
    size_t bufsize;
    char cwd[1024];

    buf = NULL;
    if (!getcwd(cwd, sizeof(cwd)))
        perror("getcwd");
    printf("%s $>", cwd);

    if (getline(&buf, &bufsize, stdin) == -1)
    {
        free(buf);
        buf = NULL;
        if (feof(stdin)) // when enter ctl+D
            return (NULL);
        else
            printf("getline failed");
    }
    return (buf);
}

/* tokenize the input line
    - parse the args
*/
char    **cell_split_line(char *line)
{
    char            **tokens;
    unsigned int    position;
    size_t          bufsize;

    bufsize = BUFSIZ;
    position = 0;
    tokens = malloc(bufsize * sizeof(*tokens));
    if (!tokens)
        return (NULL);

    for (char *token = strtok(line, DEL); token; token = strtok(NULL, DEL))
    {
        tokens[position++] = token;
        if (position >= bufsize)
        {
            bufsize *= 2;
            tokens = realloc(tokens, bufsize * sizeof(*tokens));
            if (!tokens)
                return (NULL);
        }
    }
    tokens[position] = NULL;
    return (tokens);
}

/* launch external programs */
void launch_execution(char **args)
{
    if (Fork() == CHILD_PROCESS)
        Execvp(args[0], args);
    else
        Wait(&status);
}

/* execute shell
    - init the env variables
    - if it's build in function, call it
    - if not, launch external programs
*/
void execute_shell(char **args, t_env *env, char **envp)
{
    int i;

    // input control
    if (!args[0])
        return ;
    // init env var
    if (!env->env_var)
        init_env(env, envp);
    // if builtin functions
    i = 0;
    while (g_builtin[i].builtin_name)
    {
        if (strcmp(args[0], g_builtin[i].builtin_name) == 0) // TODO replace strcmp
        {
            g_builtin[i].func(args, env, envp);
            return ;
        }
        i++;
    }
    // if not, launch external programs
    launch_execution(args);
}

void shell_loop(t_env *env, char **envp)
{
    char *line;
    char **args;

    while (1)
    {
        // 1. handle signal
        // 2. read line from command
        line = read_line();
        if (line == NULL)
        {
            printf("\nexit\n");
            break;
        }
        // check the read_line function
        // printf("you entered: %s\n", line);
        // 3. parse the args
        args = cell_split_line(line);
        // check if the args are tokenized
        // int i = 0;
        // while (args[i])
        // {
        //     printf("token[%d]: %s\n", i, args[i]);
        //     i++;
        // }
        // 4. execute the command
        execute_shell(args, env, envp);
        // 5. add history


        // 6. cleanup
        free(line);
        free(args);
    }
}

int main(int ac, char **av, char **envp)
{    
    t_env env;
    
    (void)ac;
    (void)av;
    shell_loop(&env, envp);
    return (0);
}