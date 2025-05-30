/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:08:52 by czhu              #+#    #+#             */
/*   Updated: 2025/03/29 16:08:52 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include "../incl/minishell.h"

typedef struct s_env
{
	char		**env_var;
	int			*exported;
	long		exit_status;
	int			at_prompt;
	int			heredoc_interrupted;
	int			exit_requested;
	int			exit;
}	t_env;

typedef struct s_builtin
{
	char		*builtin_name;
	void		(*func)(char **args, t_env *env);
}		t_builtin;

/* ft_echo */
void			ft_echo(char **args, t_env *env);

/* ft_pwd */
void			ft_pwd(char **args, t_env *env);

/* ft_exit */
void			ft_exit(char **args, t_env *env);
int				is_digit(char *str);

/* ft_env */
void			ft_env(char **args, t_env *env);

/* ft_unset */
void			ft_unset(char **av, t_env *env);
int				is_valid_name(char *name);
void			ft_unsetenv(char *name, t_env *env);

/* ft_cd */
void			ft_cd(char **args, t_env *env);

/* ft_export */
void			ft_export(char **args, t_env *env);

/* built-in utils */
char			*create_env_entry(char *key, char *value);
char			**env_realloc(char **old_env, int new_size);
void			add_env(char *key, char *value, t_env *env);
void			update_env(char *key, char *value, t_env *env);

/* init builtin */
t_builtin		*init_builtin(void);
void			init_env(t_env *env, char **envp);
void			print_env(t_env *env);
void			free_env(t_env *env);
int				count_env(t_env *env);

void			add_shlvl(t_env *env);

#endif
