#include "simpleshell.h"

t_builtin   g_builtin[] = 
{
    // {.builtin_name="echo", .foo=cell_echo},
    {.builtin_name="env", .foo=cell_env},
    {.builtin_name="exit", .foo=cell_exit},
    {.builtin_name=NULL},
};

int status = 0;

char    *cell_read_line(void)
{
    char    *buf;
    size_t  bufsize;
    char    cwd[BUFSIZ];

    buf = NULL;
    Getcwd(cwd, sizeof(cwd));
    printf("%s $>", cwd);

    if (getline(&buf, &bufsize, stdin) == -1)
    {
        free(buf);
        buf = NULL;
        if (feof(stdin))
            printf("[EOF]");
        else
            printf("Getline failed");
    }
    return (buf);
}

char    **cell_split_line(char *line)
{
    char            **tokens;
    unsigned int    position;
    size_t          bufsize;

    bufsize = BUFSIZ;
    position = 0;
    tokens = Malloc(bufsize * sizeof(*tokens));
    for (char *token = strtok(line, DEL); token; token = strtok(NULL, DEL))
    {
        tokens[position++] = token;
        if (position >= bufsize)
        {
            bufsize *= 2;
            tokens = Realloc(tokens, bufsize * sizeof(*tokens));
        }
    }
    tokens[position] = NULL;
    return (tokens);
}

/* replace the current process img with a new process img
    - v for vector, p for path
    - take an array of args and use PATH to find the executable
    - eg. char *args[] = {"ls", "-l", "-a", NULL};
    - execvp("ls", args);
*/
void    cell_launch(char **args)
{
    if (Fork() == CELL_Jr)
        Execvp(args[0], args);
    else
        Wait(&status);
}

/*
    - if built in, call it
    - else
        fork + execvp + wait
*/
void    cell_exec(char **args)
{
    int i = 0;
    const char  *curr;
    
    while ((curr = g_builtin[i].builtin_name))
    {
        if (!strcmp(curr, args[0]))
        {
            g_builtin[i].foo(args);
            return ;
        }
        i++;
    }
    cell_launch(args); // fork, execvp, wait
}

int main()
{
    char    *line;
    char    **args;

    // REPL: read, evaluate, print, loop
    while ((line = cell_read_line()))
    {
        // 1) read: prompt + get line
        printf("%s\v", line);

        // 2) evaluate: gettok
        // lexing -> parsing
        args = cell_split_line(line);
        for (int i = 0; args[i]; ++i)
            printf("%s\n", args[i]);

        // 3) execu
        cell_exec(args);

        // 4) freeup
        free(line);
        free(args);

    }
    return (0);
}
