# include "../incl/execution.h"

/* fork function wrapper */
pid_t   Fork(void)
{
    pid_t   pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    return (pid);
}

/* execve function wrapper
    - check input: path, av
    - check env init
    - use execve to execute the cmd
        - if execve fails (returns -1), print error and exit
*/
void Execve(char *path, char **av, t_env *env)
{
    if (!path || !av)
    {
        printf("execve: invalid args\n");
        exit(1);
    }
    if (!env || !env->env_var)
    {
        printf("env not initialized\n");
        exit(1);
    }
    if (execve(path, av, env->env_var) == -1)
    {
        perror("execve failed");
        exit(126);
    }
}

/* wait function wrapper
    - check status
    - if wait fails, print error and return result
*/
pid_t   Wait(int *status)
{
    pid_t   result;

    if (!status)
    {
        printf("wait: status arg required\n");
        return (-1);
    }
    result = wait(status);
    if (result == -1)
        perror("wait failed");
    return (result);
}