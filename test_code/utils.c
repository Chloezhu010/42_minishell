#include "simpleshell.h"

void    Getcwd(char *buf, size_t bufsize)
{
    if (!getcwd(buf, bufsize))
        perror("getcwd failed");
}

void    *Malloc(size_t size)
{
    void    *ptr;

    if (size == 0)
        return (NULL);
    ptr = malloc(size);
    if (!ptr)
    {
        perror("malloc failed");
        exit(1);
    }
    return (ptr);
}

void    *Realloc(void *ptr, size_t size)
{
    void    *new_ptr;

    new_ptr = realloc(ptr, size);
    if (!new_ptr && size != 0)
    {
        perror("realloc failed");
        exit(1);
    }
    return (new_ptr);
}

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

void    Execvp(const char *file, char *const av[])
{
    if (!file || !av)
    {
        fprintf(stderr, "execvp: invalid args\n");
        exit(1);
    }
    if (execvp(file, av) == -1)
    {
        perror("cell_jr failed");
        exit(1);
    }
}

pid_t   Wait(int *status)
{
    pid_t   result;

    if (!status)
    {
        fprintf(stderr, "wait: status arg required\n");
        return (-1);
    }
    result = wait(status);
    if (result == -1)
        perror("wait failed");
    return (result);
}

int cell_exit(char **args)
{
    (void)(**args);
    exit(0);
}

int cell_env(char **args)
{
    extern char **environ;

    (void)args;
    if (!environ)
        return (1);
    for (int i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
    return (0);
}