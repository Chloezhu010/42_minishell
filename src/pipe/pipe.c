#include "../../incl/minishell.h"

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

/* helper function: check if input files exist for a cmd
    - return 1 for error
    - return 0 for success
*/
int check_input_file(t_cmd *cmd, t_env *env)
{
    t_redir *redir;
    int fd;

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIRECT_IN)
        {
            fd = open(redir->file, O_RDONLY);
            if (fd == -1)
            {
                perror("minishell");
                env->exit_status = 1;
                return (1);
            }
            close(fd);
        }
        redir = redir->next;
    }
    return (0);
}

/* helper function: create output files for a cmd */
int create_output_file(t_cmd *cmd, t_env *env)
{
    t_redir *redir;
    int fd;

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_REDIRECT_APPEND)
        {
            if (redir->type == TOKEN_REDIRECT_APPEND)
                fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("minishell");
                env->exit_status = 1;
                return (1);
            }
            close(fd);
        }
        redir = redir->next;
    }
    return (0);
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
    int pipefd[2];
    pid_t pid;
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO); // add stdout backup
    int stdin_backup_child = -1; // add for child process
    int stdout_backup_child = -1; // add for child process
    pid_t pids[64];
    int pid_count = 0;
    int prev_pipe_read = -1;
    int input_error = 0;

    current = cmds;
    while (current)
    {
        /* only create pipe if not the last cmd */
        if (current->next)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                env->exit_status = 1;
                break;
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            env->exit_status = 1;
            break;
        }

        if (pid == 0)  // Child process
        {
            /* check input files first */
            input_error = check_input_file(current, env);
            if (input_error)
            {
                if (current->next)
                {
                    close(pipefd[0]);
                    close(pipefd[1]);
                }
                exit(1);
            }
            /* only create output file if input file exists */
            if (create_output_file(current, env))
                exit(1);
            // handle input from previous pipe
            if (prev_pipe_read != -1)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            /* add handle input redirect */
            else if (current->infile || (current->heredoc && current->fd_in > 0))
            {
                if (handle_input_redirect(current, &stdin_backup_child, env) == -1)
                    exit(1);
            }
            // 处理输出重定向
            if (current->next)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            /* add handle output redirect */
            else if (current->outfile)
            {
                if (handle_output_redirect(current, &stdout_backup_child, env) == -1)
                    exit(1);
            }
            execute_shell(current, env);
            /* add restore */
            restore_io(stdin_backup_child, stdout_backup_child);

            exit(env->exit_status);
        }
        else  // Parent process
        {
            pids[pid_count++] = pid;

            // 关闭上一个管道的读端
            if (prev_pipe_read != -1)
                close(prev_pipe_read);

            // 如果不是最后一个命令，准备下一个管道的读端
            if (current->next)
            {
                close(pipefd[1]);
                prev_pipe_read = pipefd[0];
            }
        }
        current = current->next;
    }

    // 恢复标准输入
    /* add stdout restoration */
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);

    // 等待所有子进程
    for (int i = 0; i < pid_count; i++)
    {
        int status;
        while (waitpid(pids[i], &status, 0) == -1 && errno == EINTR)
            ; // 重试直到成功
        if (WIFEXITED(status))
            env->exit_status = WEXITSTATUS(status);
    }
}