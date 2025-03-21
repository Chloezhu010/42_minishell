#include "../../incl/pipe.h"

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

/* init the pipe struct */
void init_pipe(t_pipe *context)
{
    context->pid_count = 0;
    context->prev_pipe_read = -1;
    context->last_pid = -1;
    context->stdin_backup = dup(STDIN_FILENO);
    context->stdout_backup = dup(STDOUT_FILENO);
    context->stderr_backup = dup(STDERR_FILENO);
}

/* mark all cmd as part of pipeline */
void mark_pipeline_cmd(t_cmd *cmd)
{
    t_cmd *current;

    current = cmd;
    while (current)
    {
        current->in_pipe = 1;
        current = current->next;
    }
}

/* create pipe and handle error */
int create_cmd_pipe(t_pipe *ctx, t_env *env)
{
    if (pipe(ctx->pipefd) == -1)
    {
        perror("pipe");
        env->exit_status = 1;
        return (1);
    }
    return (0);
}

/* handle redirect error
    - if there is a next cmd
        - close the read & write end of the pipe
        - exit 1
*/
void handle_redirect_error(t_cmd *cmd, t_pipe *ctx, int redirect_error)
{
    if (redirect_error)
    {
        if (cmd->next)
        {
            close(ctx->pipefd[0]);
            close(ctx->pipefd[1]);
        }
        exit(1);
    }
}

/* handle redirect stderr to /dev/null for non-last cmd */
void handle_stderr_redirect(t_cmd *cmd)
{
    int null_fd;
    
    if (cmd->next)
    {
        null_fd = open("/dev/null", O_WRONLY);
        if (null_fd != -1)
        {
            dup2(null_fd, STDERR_FILENO);
            close(null_fd);
        }
    }
}

/* setup pipe input
    - handle input from pipeline
        - if the current cmd is part of a pipeline, read from its prev cmd
        - current cmd read its input from the prev cmd
        - close the prev_pipe_read
    - else handle input redirect
        - if redirect fails, close both read & write end of pipe
    - return 1 on error, 0 on success
*/
int setup_pipe_input(t_cmd *cmd, t_pipe *ctx, int *stdin_backup, t_env *env)
{
    if (cmd->infile || (cmd->heredoc && cmd->fd_in > 0))
    {
        if (handle_input_redirect(cmd, stdin_backup, env) == -1)
        {
            if (cmd->next)
            {
                close(ctx->pipefd[0]);
                close(ctx->pipefd[1]);
            }
            return (1);
        }
    }
    else if (ctx->prev_pipe_read != -1)
    {
        dup2(ctx->prev_pipe_read, STDIN_FILENO);
        close(ctx->prev_pipe_read);
    }
    return (0);
}

/* setup pipe output
    - handle output for pipeline
        - if there is a next cmd
        - redirect the stdout to the write end of the pipe
        - close both end of the pipe
    - handle output redirect
    - return 1 on error, 0 on success
*/
int setup_pipe_output(t_cmd *cmd, t_pipe *ctx, int *stdout_backup, t_env *env)
{
    if (cmd->next)
    {
        dup2(ctx->pipefd[1], STDOUT_FILENO);
        close(ctx->pipefd[0]);
        close(ctx->pipefd[1]);
    }
    if (cmd->outfile)
    {
        if (handle_output_redirect(cmd, stdout_backup, env) == -1)
            return (1);
    }
    return (0);
}

/* execute the child process in pipe */
void execute_pipe_child(t_cmd *current, t_pipe *ctx, t_env *env, int redirect_error)
{
    int stdin_backup_child;
    int stdout_backup_child;

    stdin_backup_child = -1;
    stdout_backup_child = -1;
    /* handle redirect error */
    handle_redirect_error(current, ctx, redirect_error);
    /* setup stderr redirect for non-last cmd */
    handle_stderr_redirect(current);
    /* setup input  & output redirect */
    if (setup_pipe_input(current, ctx, &stdin_backup_child, env))
        exit(1);
    if (setup_pipe_output(current, ctx, &stdout_backup_child, env))
        exit(1);
    /* execute cmd */
    execute_cmd(current, env);
    /* restore io */
    restore_io(stdin_backup_child, stdout_backup_child);
    /* exit */
    exit(env->exit_status);
}

/* handle parent process after fork
    - close prev pipe read end
    - if not the last cmd, prepare next pipe read end
    - track last cmd's pid
*/
void execute_parent_process(t_pipe *ctx, t_cmd *cmd, pid_t pid)
{
    ctx->pids[ctx->pid_count++] = pid;
    
    if (ctx->prev_pipe_read != -1)
        close(ctx->prev_pipe_read);
    if (cmd->next)
    {
        close(ctx->pipefd[1]);
        ctx->prev_pipe_read = ctx->pipefd[0];
    }
    if (!cmd->next)
        ctx->last_pid = pid;
}

/* restore std fd */
void restore_std_fd(t_pipe *ctx)
{
    dup2(ctx->stdin_backup, STDIN_FILENO);
    dup2(ctx->stdout_backup, STDOUT_FILENO);
    dup2(ctx->stderr_backup, STDERR_FILENO);
    close(ctx->stdin_backup);
    close(ctx->stdout_backup);
    close(ctx->stderr_backup);
}

/* wait for all child process and set exit status */
void wait_for_child(t_pipe *ctx, t_env *env)
{
    int i;
    int status;
    int exit_code;

    i = 0;
    while (i < ctx->pid_count)
    {
        waitpid(ctx->pids[i], &status, 0);
        if (ctx->pids[i] == ctx->last_pid && WIFEXITED(status))
        {
            exit_code = WEXITSTATUS(status);
            if (exit_code == 2)
                env->exit_status = 1;
            else
                env->exit_status = exit_code;
        }
        i++;
    }
}

/* main pipeline execution
    - init pipeline ctx and mark cmds
    - check redirect before creating pipes
    - create pipe if not the last cmd
    - fork process
    - restore std fd
    - wait for all child process
*/
void execute_pipeline(t_cmd *cmd, t_env *env)
{
    t_cmd *current;
    t_pipe ctx;
    pid_t pid;
    int redirect_error;

    init_pipe(&ctx);
    mark_pipeline_cmd(cmd);

    current = cmd;
    while (current)
    {
        redirect_error = process_redirect(current, env);
        if (current->next)
        {
            if (create_cmd_pipe(&ctx, env))
                break;
        }
        // fork process
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            env->exit_status = 1;
            break;
        }
        if (pid == CHILD_PROCESS)
            execute_pipe_child(current, &ctx, env, redirect_error);
        else
            execute_parent_process(&ctx, current, pid);
        current = current->next;
    }
    restore_std_fd(&ctx);
    wait_for_child(&ctx, env);
}

// void execute_pipeline(t_cmd *cmds, t_env *env)
// {
//     t_cmd *current;
//     int pipefd[2];
//     pid_t pid;
//     int stdin_backup = dup(STDIN_FILENO);
//     int stdout_backup = dup(STDOUT_FILENO); // add stdout backup
//     int stderr_backup = dup(STDERR_FILENO); // add stderr backup
//     int stdin_backup_child = -1; // add for child process
//     int stdout_backup_child = -1; // add for child process
//     pid_t pids[64];
//     int pid_count = 0;
//     int prev_pipe_read = -1;
//     int last_pid = -1;
//     int redirect_error;
//     int has_redirect_error[64] = {0};
//     current = cmds;
//     while (current)
//     {
//         /* check redirect before creating pipe */
//         redirect_error = process_redirect(current, env);
//         if (redirect_error)
//             has_redirect_error[pid_count] = 1;
//         /* only create pipe if not the last cmd */
//         if (current->next)
//         {
//             if (pipe(pipefd) == -1)
//             {
//                 perror("pipe");
//                 env->exit_status = 1;
//                 break;
//             }
//         }
//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork");
//             env->exit_status = 1;
//             break;
//         }
//         if (pid == 0)  // Child process
//         {             
//             /* if redirect error, close fd and exit */
//             if (redirect_error)
//             {
//                 if (current->next)
//                 {
//                     close(pipefd[0]);
//                     close(pipefd[1]);
//                 }
//                 exit(1);
//             }
//             /* for non-last cmd, redirect stderr to /dev/null if it's not the last cmd */
//             if (current->next)
//             {
//                 int null_fd = open("/dev/null", O_WRONLY);
//                 if (null_fd != -1)
//                 {
//                     dup2(null_fd, STDERR_FILENO);
//                     close(null_fd);
//                 }
//             }
//             // handle input from previous pipe
//             if (prev_pipe_read != -1)
//             {
//                 // stdin_backup_child = dup(STDIN_FILENO);
//                 dup2(prev_pipe_read, STDIN_FILENO);
//                 close(prev_pipe_read);
//             }
//             /* add handle input redirect */
//             else if (current->infile || (current->heredoc && current->fd_in > 0))
//             {
//                 if (handle_input_redirect(current, &stdin_backup_child, env) == -1)
//                 {
//                     /* close pipe ends if redirect fails */
//                     if (current->next)
//                     {
//                         close(pipefd[0]);
//                         close(pipefd[1]);
//                     }
//                     exit(1);
//                 }
//             }
//             // 处理输出重定向
//             if (current->next)
//             {
//                 // stdout_backup_child = dup(STDOUT_FILENO);
//                 dup2(pipefd[1], STDOUT_FILENO);
//                 close(pipefd[0]);
//                 close(pipefd[1]);
//             }
//             /* add handle output redirect */
//             if (current->outfile)
//             {
//                 if (handle_output_redirect(current, &stdout_backup_child, env) == -1)
//                     exit(1);
//             }
//             /* execute cmd directly in pipe */
//             execute_cmd(current, env);
//             // /* execute the cmd */
//             // current->in_pipe = 1;
//             // execute_shell(current, env);
//             /* add restore */
//             restore_io(stdin_backup_child, stdout_backup_child);
//             exit(env->exit_status);
//         }
//         else  // Parent process
//         {
//             pids[pid_count++] = pid;
//             // 关闭上一个管道的读端
//             if (prev_pipe_read != -1)
//                 close(prev_pipe_read);
//             // 如果不是最后一个命令，准备下一个管道的读端
//             if (current->next)
//             {
//                 close(pipefd[1]);
//                 prev_pipe_read = pipefd[0];
//             }
//             if (!current->next)
//                 last_pid = pid;
//         }
//         current = current->next;
//     }
//     // 恢复标准输入
//     /* add stdout restoration */
//     dup2(stdin_backup, STDIN_FILENO);
//     dup2(stdout_backup, STDOUT_FILENO);
//     dup2(stderr_backup, STDERR_FILENO);
//     close(stdin_backup);
//     close(stdout_backup);
//     close(stderr_backup);
//     /* wait for all child processes, but only set exit status from the last cmd */
//     for (int i = 0; i < pid_count; i++)
//     {
//         int status;      
//         waitpid(pids[i], &status, 0);
//         if (pids[i] == last_pid && WIFEXITED(status))
//         {
//             int exit_code = WEXITSTATUS(status);
//             if (exit_code == 2)
//                 env->exit_status = 1;
//             else
//                 env->exit_status = exit_code;
//         }
//     }
// }