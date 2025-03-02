#include "../incl/minishell.h"

void    *ft_malloc(size_t size)
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

int ft_strcmp(const char *s1, const char *s2)
{
    while ((*s1 || *s2) && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}