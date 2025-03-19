#include "../../incl/minishell.h"

/*
    - create a temp file to store input line
        - write only
        - if not exits, create one
        - if the file exists, content will be erased
    - read user input until the delimiter
        - input lines are read & written to the temp file
        - close and reopen with read-only mode
    - return the fd for redirection
*/
int handle_heredoc(char *delimiter, t_env *env)
{
    char *line;
    int fd;
    
    /* create a temp file */
    fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        env->exit_status = 1;
        return (-1);
    }
    /* read input until the delimiter */
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
            break ;
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    /* reopen the file */
    fd = open("/tmp/minishell_heredoc", O_RDONLY);
    return (fd);
}

/* handle input redirect
    - return -1 for error, 0 for success
*/
int handle_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env)
{
    int fd;
    
    if (!cmd->heredoc && !cmd->infile)
    {
        *stdin_backup = -1;
        return (0);
    }
    *stdin_backup = dup(STDIN_FILENO);
    if (*stdin_backup == -1)
    {
        perror("dup");
        env->exit_status = 1;
        return (-1);
    }
    /* handle heredoc */
    if (cmd->heredoc && cmd->fd_in > 0)
    {
        if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(*stdin_backup);
            env->exit_status = 1;
            return (-1);
        }
        close(cmd->fd_in);
    }
    /* handle other input redirect */
    else if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd == -1)
        {
            perror("minishell");
            close(*stdin_backup);
            env->exit_status = 1;
            return (-1);
        }
        /* redirect stdin to the input file */
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            close(*stdin_backup);
            env->exit_status = 1;
            return (-1);
        }
        close(fd);
    }
    return (0);
}

/* handle output redirect TODO */
int handle_output_redirect(t_cmd *cmd, int *stdout_backup, t_env *env)
{
    int fd;
    
    if (!cmd->outfile)
    {
        *stdout_backup = -1;
        return (0);
    }
    *stdout_backup = dup(STDOUT_FILENO);
    if (*stdout_backup == -1)
    {
        perror("dup");
        env->exit_status = 1;
        return (-1);
    }
    if (cmd->append)
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell");
        close(*stdout_backup);
        env->exit_status = 1;
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell");
        close(fd);
        close(*stdout_backup);
        env->exit_status = 1;
        return (-1);
    }
    close(fd);
    return (0);
}

/* restore std input after execution */
void restore_io(int stdin_backup, int stdout_backup)
{
    if (stdin_backup != -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
    if (stdout_backup != -1)
    {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
    }
}

/* process all redirect in order (from left to right) 
    - check if files exist/ can be created, and handle permissions
    - return 0 on success, 1 on error
*/
int process_redirect(t_cmd *cmd, t_env *env)
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
                /* only set the exit status if it's not in a pipe */
                if (!cmd->in_pipe)
                    env->exit_status = 1;
                return (1);
            }
            close(fd);
        }
        else if (redir->type == TOKEN_REDIRECT_APPEND || redir->type == TOKEN_REDIRECT_OUT)
        {
            if (redir->type == TOKEN_REDIRECT_APPEND)
                fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("minishell");
                /* only set the exit status if it's not in a pipe */
                if (!cmd->in_pipe)
                    env->exit_status = 1;
                return (1);
            }
            close(fd);
        }
        redir = redir->next;
    }
    return (0);
}

// // === test redirect ===
// int main()
// {
//     t_cmd cmd;
//     int stdin_backup = -1;
//     int stdout_backup = -1;

//     // Initialize the command structure
//     cmd.infile = NULL;
//     cmd.outfile = NULL;
//     cmd.append = 0;
//     cmd.heredoc = 0;
//     cmd.delimiter = NULL;
//     cmd.fd_in = -1;

//     // === test input redirect (cat < input.txt)
//     printf("test input redirection\n");
//     cmd.infile = "input.txt";
//     if (handle_input_redirect(&cmd, &stdin_backup) == -1)
//     {
//         fprintf(stderr, "input redirect failed\n");
//         return (1);
//     }
//     // simulate a cmd that reads from stdin
//     char buffer[1024];
//     ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
//     if (bytes_read > 0)
//     {
//         buffer[bytes_read] = '\0';
//         printf("read from input file: %s\n", buffer);
//     }
//     else
//         perror("read");
//     restore_io(stdin_backup, stdout_backup);
//     // === test output rediret
//     printf("\ntest output redirection\n");
//     cmd.outfile = "output.txt";
//     cmd.append = 0;
//     if (handle_output_redirect(&cmd, &stdout_backup) == -1)
//     {
//         fprintf(stderr, "output redirect failed\n");
//         return (1);
//     }
//     // simulate a cmd that writes to stdout
//     printf("hello \n");
//     restore_io(stdin_backup, stdout_backup);

//     // === test append mode
//     printf("\ntest output redirection - append mode\n");
//     cmd.outfile = "output.txt";
//     cmd.append = 1;
//     if (handle_output_redirect(&cmd, &stdout_backup) == -1)
//     {
//         fprintf(stderr, "output redirect failed\n");
//         return (1);
//     }
//     printf("school42\n");
//     restore_io(stdin_backup, stdout_backup);
//     // === test heredoc
//     printf("\ntest heredoc\n");
//     cmd.heredoc = 1;
//     cmd.delimiter = "eof";
//     cmd.fd_in = handle_heredoc(cmd.delimiter);
//     if (cmd.fd_in == -1)
//     {
//         fprintf(stderr, "heredoc failed\n");
//         return (1);
//     }
//     // simuate a cmd that reads from stdin
//     bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
//     if (bytes_read > 0)
//     {
//         buffer[bytes_read] = '\0';
//         printf("read from input file: %s\n", buffer);
//     }
//     else
//         perror("read");
//     restore_io(stdin_backup, stdout_backup);
// }