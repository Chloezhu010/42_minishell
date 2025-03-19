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

// /* init the pipe struct */
// void init_pipe(t_pipe *context)
// {
//     context->pid_count = 0;
//     context->prev_pipe_read = -1;
//     context->stdin_backup = dup(STDIN_FILENO);
//     context->stdout_backup = dup(STDOUT_FILENO);
// }

// /* setup input for a cmd in pipeline */
// void setup_pipe_input(t_cmd *cmd, t_pipe *ctx, t_env *env)
// {
//     int stdin_backup = -1;

//     /* handle input from prev pipe */
//     if (ctx->prev_pipe_read != -1)
//     {
//         stdin_backup = dup(STDIN_FILENO);
//         dup2(ctx->prev_pipe_read, STDIN_FILENO);
//         close(ctx->prev_pipe_read);
//     }
//     /* handle input redirect if not getting input from a pipe */
//     else if (cmd->infile || (cmd->heredoc && cmd->fd_in > 0))
//     {
//         if (handle_input_redirect(cmd, &stdin_backup, env) == -1)
//             exit(1);
//     }
//     /* store the backup fd in the context */
//     ctx->stdin_backup = stdin_backup;
// }

// /* setup output for a cmd in pipeline */
// void setup_pipe_output(t_cmd *cmd, t_pipe *ctx, t_env *env)
// {
//     int stdout_backup = -1;
//     int fd;
    
//     /* determine where output should go */
//     if (cmd->next)
//     {
//         /* if there is a next cmd, output goes to the pipe */
//         stdout_backup = dup(STDOUT_FILENO);
//         dup2(ctx->pipefd[1], STDOUT_FILENO);
//         close(ctx->pipefd[0]);
//     }
//     /* handle output redirect regardless of pipe */
//     if (cmd->outfile)
//     {
//         if (cmd->append)
//             fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         else
//             fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (fd == -1)
//         {
//             perror("minishell");
//             env->exit_status = 1;
//             exit(1);
//         }
//         /* save original stdout, and redirect stdout to the file */
//         if (stdout_backup == -1)
//             stdout_backup = dup(STDOUT_FILENO);
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }
//     /* store the backup fd in the context */
//     ctx->stdout_backup = stdout_backup;
// }

// /* restore the redirected fd */
// void restore_pipe_io(t_pipe *ctx)
// {
//     if (ctx->stdin_backup != -1)
//     {
//         dup2(ctx->stdin_backup, STDIN_FILENO);
//         close(ctx->stdin_backup);
//         ctx->stdin_backup = -1;
//     }
    
//     if (ctx->stdout_backup != -1)
//     {
//         dup2(ctx->stdout_backup, STDOUT_FILENO);
//         close(ctx->stdout_backup);
//         ctx->stdout_backup = -1;
//     }
// }

// /* create a pipe and handle error */
// int create_pipe(t_pipe *ctx, t_env *env)
// {
//     if (pipe(ctx->pipefd) == -1)
//     {
//         perror("pipe");
//         env->exit_status = 1;
//         return (1);
//     }
//     return (0);
// }

// /* fork a process and handle error */
// pid_t fork_process(t_env *env)
// {
//     pid_t pid;

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         env->exit_status = 1;
//     }
//     return (pid);
// }

// /* handle child process execution */
// void execute_pipe_child(t_cmd *current, t_pipe *ctx, t_env *env)
// {
//     /* setup input/ output redirect */
//     setup_pipe_input(current, ctx, env);
//     setup_pipe_output(current, ctx, env);
//     /* execute the cmd */
//     execute_shell(current, env);
//     /* restore fd */
//     restore_pipe_io(ctx);
//     /* close all pipe fd */
//     if (current->next && ctx->pipefd[1] != -1)
//         close(ctx->pipefd[1]);
//     exit(env->exit_status);
// }

// /* handle parent process in pipe */
// void handle_pipe_parent(t_pipe *ctx, t_cmd *current, pid_t pid)
// {
//     ctx->pids[ctx->pid_count++] = pid;
//     /* close previous pipe read end */
//     if (ctx->prev_pipe_read != -1)
//         close(ctx->prev_pipe_read);
//     /* if not the last cmd, prepare next pipe read end */
//     if (current->next)
//     {
//         close(ctx->pipefd[1]);
//         ctx->prev_pipe_read = ctx->pipefd[0];
//     }
// }

// /* wait for all process in pipe */
// void wait_all_pipe(t_pipe *ctx, t_env *env)
// {
//     int status;
//     int last_status = 0;
    
//     for (int i = 0; i < ctx->pid_count; i++)
//     {
//         int status;
//         waitpid(ctx->pids[i], &status, 0);
//         if (WIFEXITED(status))
//             env->exit_status = WEXITSTATUS(status);
//     }
// }

// /* cleanup pipe */
// void cleanup_pipe(t_pipe *ctx)
// {
//     /* restore stdin and stdout */
//     if (ctx->stdin_backup != -1)
//     {
//         dup2(ctx->stdin_backup, STDIN_FILENO);
//         close(ctx->stdin_backup);
//     }
//     if (ctx->stdout_backup != -1)
//     {
//         dup2(ctx->stdout_backup, STDOUT_FILENO);
//         close(ctx->stdout_backup);
//     }
//     /* close any remaining pipe fd */
//     if (ctx->prev_pipe_read != -1)
//         close(ctx->prev_pipe_read);
// }

// /* main pipe execution function */
// void execute_pipeline(t_cmd *cmds, t_env *env)
// {
//     t_cmd *current;
//     t_pipe ctx;
//     pid_t pid;

//     init_pipe(&ctx);
//     /* mark all cmds as part of a pipe */
//     current = cmds;
//     while (current)
//     {
//         current->in_pipe = 1;
//         current = current->next;
//     }
//     current = cmds;
//     while (current)
//     {
//         /* only create pipe if not the last cmd */
//         if (current->next)
//         {
//             if (create_pipe(&ctx, env))
//                 break;
//         }
//         pid = fork_process(env);
//         if (pid == -1)
//             break;
//         /* if child process */
//         if (pid == 0)
//             execute_pipe_child(current, &ctx, env);
//         else
//             handle_pipe_parent(&ctx, current, pid);
//         current = current->next;
//     }
//     cleanup_pipe(&ctx);
//     wait_all_pipe(&ctx, env);
// }
void execute_pipeline(t_cmd *cmds, t_env *env)
{
    t_cmd *current;
    int pipefd[2];
    pid_t pid;
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO); // add stdout backup
    int stderr_backup = dup(STDERR_FILENO); // add stderr backup
    int stdin_backup_child = -1; // add for child process
    int stdout_backup_child = -1; // add for child process
    pid_t pids[64];
    int pid_count = 0;
    int prev_pipe_read = -1;
    int last_pid = -1;
    int redirect_error;
    int has_redirect_error[64] = {0};

    current = cmds;
    while (current)
    {
        /* check redirect before creating pipe */
        redirect_error = process_redirect(current, env);
        if (redirect_error)
            has_redirect_error[pid_count] = 1;
        
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
            /* if redirect error, close fd and exit */
            if (redirect_error)
            {
                if (current->next)
                {
                    close(pipefd[0]);
                    close(pipefd[1]);
                }
                exit(1);
            }

            /* for non-last cmd, redirect stderr to /dev/null if it's not the last cmd */
            if (current->next)
            {
                int null_fd = open("/dev/null", O_WRONLY);
                if (null_fd != -1)
                {
                    dup2(null_fd, STDERR_FILENO);
                    close(null_fd);
                }
            }

            // handle input from previous pipe
            if (prev_pipe_read != -1)
            {
                // stdin_backup_child = dup(STDIN_FILENO);
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            /* add handle input redirect */
            else if (current->infile || (current->heredoc && current->fd_in > 0))
            {
                if (handle_input_redirect(current, &stdin_backup_child, env) == -1)
                {
                    /* close pipe ends if redirect fails */
                    if (current->next)
                    {
                        close(pipefd[0]);
                        close(pipefd[1]);
                    }
                    exit(1);
                }
            }
            // 处理输出重定向
            if (current->next)
            {
                // stdout_backup_child = dup(STDOUT_FILENO);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            /* add handle output redirect */
            if (current->outfile)
            {
                if (handle_output_redirect(current, &stdout_backup_child, env) == -1)
                    exit(1);
            }

            /* execute the cmd */
            current->in_pipe = 1;
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
            if (!current->next)
                last_pid = pid;
        }
        current = current->next;
    }

    // 恢复标准输入
    /* add stdout restoration */
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    dup2(stderr_backup, STDERR_FILENO);
    close(stdin_backup);
    close(stdout_backup);
    close(stderr_backup);

    /* wait for all child processes, but only set exit status from the last cmd */
    for (int i = 0; i < pid_count; i++)
    {
        int status;
        
        waitpid(pids[i], &status, 0);
        if (pids[i] == last_pid && WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 2)
                env->exit_status = 1;
            else
                env->exit_status = exit_code;
        }
    }
}