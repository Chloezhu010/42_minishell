#ifndef BUILTIN_H
# define BUILTIN_H
# include "../incl/minishell.h"

typedef struct s_env
{
    char	**env_var;
    int *exported;
}		t_env;

typedef struct s_builtin
{
    char *builtin_name;
    void (*func)(char **args, t_env *env, char **envp);
}   t_builtin;

/* ft_echo */
void	ft_echo(char **args, t_env *env, char **envp);
/* ft_pwd */
void	ft_pwd(char **args, t_env *env, char **envp);
/* ft_exit */
void	ft_exit(char **args, t_env *env, char **envp);
int		is_digit(char *str);
/* ft_env */
void	ft_env(char **args, t_env *env, char **envp);
/* ft_unset */
void	ft_unset(char **av, t_env *env, char **envp);
int		ft_env_len(t_env *env);
int		is_valid_name(char *name);
void	ft_unsetenv(char *name, t_env *env);
/* ft_cd */
void ft_cd(char **args, t_env *env, char **envp);
/* ft_export */
void ft_export(char **args, t_env *env, char **envp);

/* built-in utils */
void	init_env(t_env *env, char **envp);
void	print_env(t_env *env);
void	free_env(t_env *env);
char *create_env_entry(char *key, char *value);
void update_env(char *key, char *value, t_env *env);
#endif
