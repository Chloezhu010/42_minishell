# include "../incl/execution.h"

/* find the executable path for execve
    - find the PATH in env var
    - split path using ":" to get dir array
    - loop through the dir array + "/" + cmd to find the executable path
        - check if full_path is executable
        - if yes, return full_path
        - if not, free all, i++
    - if no executable path found, cleanup dir array and return NULL
*/
char *find_path(char *cmd)
{
    char *path;
    char **dir;
    char *tmp;
    char *full_path;
    int i;

    path = getenv("PATH");
    if (!path)
    {
        perror("getenv");
        return (NULL);
    }
    dir = ft_split(path, ':');
    i = 0;
    while (dir[i])
    {
        tmp = ft_strjoin(dir[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        i++;
    }
    ft_freeup(dir);
    free(dir);
    return (NULL);
}
// // ==== test find_path ===
// int main(int ac, char **av)
// {
//     char *full_path;

//     (void)ac;
//     full_path = find_path(av[1]);
//     printf("%s\n", full_path);
// }