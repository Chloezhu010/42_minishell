#include "../incl/minishell.h"

/* global variable for exit status */
extern int	g_exit_status;

/* pipe function wrapper */
void ft_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return ;
    }
}

/* 
    Implementation
    - loop through each cmd
        - create pipe
        - fork a child process
        - if child process
            - close the reading end of the pipe (pipefd[0])
            - redirect stdout to the writing end of the pipe
            - execute the current cmd
            - exit the child process
        - if parent process
            - close the writing end of the pipe
            - redirect stdin to the reading end of the pipe
            - wait for the child process to finish
        - move the next cmd
    - wait for all child process to finish

    Example: "cmd 1 | cmd 2 | cmd 3"
        pipe[1] writing end, pipe[0] reading end
        -> cmd1 writes to pipe1[1]
        -> cmd2 reads from pipe1[0] and writes to pipe2[1]
        -> cmd3 reads from pipe2[0]
*/
void execute_pipeline(t_cmd *cmds, t_env *env)
{
    t_cmd *current;
    int pipefd[2];// array to hold read / write end
    pid_t pid;

    /* execute pipeline */
    current = cmds;
    while (current && current->next)
    {
        ft_pipe(pipefd);
        pid = ft_fork();
        /* if in the child process */
        if (pid == CHILD_PROCESS)
        {
            close(pipefd[0]);// close reading end
            dup2(pipefd[1], STDOUT_FILENO);// setup output to current pipe
            close(pipefd[1]);
            execute_shell(current->args, env);
            exit(g_exit_status);
        }
        else
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            waitpid(pid, &g_exit_status, 0);// wait for child process
        }
        current = current->next;
    }
    /* execute the last cmd without piping */
    execute_shell(current->args, env);
}
