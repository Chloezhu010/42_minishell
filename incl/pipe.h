#ifndef PIPE_H
# define PIPE_H

# include "../incl/minishell.h"

/* struct for pipe context */
typedef struct s_pipe
{
    pid_t pids[64];
    int pid_count;
    int prev_pipe_read;
    int pipefd[2];
    int stdin_backup;
    int stdout_backup;
    int stderr_backup;
    int last_pid;
} t_pipe;

typedef struct s_child_pipe
{
    int stdin_backup;
    int stdout_backup;
    int redirect_error;
    int *pipefd;
} t_child_pipe;


void execute_pipeline(t_cmd *cmds, t_env *env);

#endif