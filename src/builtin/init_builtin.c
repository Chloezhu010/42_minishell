/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:10:36 by czhu              #+#    #+#             */
/*   Updated: 2025/03/25 14:31:33 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* init the built_in toggle
	- used in execute_shell
*/
t_builtin	*init_builtin(void)
{
	static t_builtin	builtin_in[] = {
	{"pwd", ft_pwd},
	{"cd", ft_cd},
	{"echo", ft_echo},
	{"env", ft_env},
	{"exit", ft_exit},
	{"export", ft_export},
	{"unset", ft_unset},
	{NULL, NULL}
	};

	return (builtin_in);
}

/* count the nbr of env var */
int	count_envp(char **envp)
{
	int	count;

	count = 0;
	if (!envp)
		return (0);
	while (envp[count])
		count++;
	return (count);
}

/* init the env from envp
    - count the env variables
    - malloc for env variables
    - copy env variables to my own struct
    - null terminated
*/

void    add_shlvl(t_env *env)
{
    int     i;
    int     level;
    int     shlvl_index = -1;
    char    *shlvl_str;
    char    *new_value;
    char    *new_shlvl;

    // 查找SHLVL环境变量
    shlvl_str = NULL;
    i = 0;
    while (env->env_var && env->env_var[i])
    {
        if (ft_strncmp(env->env_var[i], "SHLVL=", 6) == 0)
        {
            shlvl_str = env->env_var[i] + 6;
            shlvl_index = i; // store the index to find shlvl
            // printf("[db]: shlvl found at index %d\n", shlvl_index);
            break;
        }
        i++;
    }

    // SHLVL不存在，添加SHLVL=1
    if (!shlvl_str)
    {
        i = 0;
        while (env->env_var && env->env_var[i])
            i++;
        new_value = ft_strdup("SHLVL=1");
        // 扩展环境变量数组
        char **new_env = (char **)ft_malloc((i + 2) * sizeof(char *));
        i = 0;
        while (env->env_var && env->env_var[i])
        {
            new_env[i] = env->env_var[i];
            i++;
        }
        new_env[i] = new_value;
        new_env[i + 1] = NULL;
        
        if (env->env_var)
            free(env->env_var);
        env->env_var = new_env;
        return;
    }

    // 检查是否包含非数字字符
    i = 0;
    if (shlvl_str[0] == '-' || shlvl_str[0] == '+')
        i++;
    while (shlvl_str[i])
    {
        if (!ft_isdigit(shlvl_str[i]))
        {
            // 非数字，设置为1
            new_shlvl = ft_strdup("SHLVL=1");
            if (shlvl_index >= 0)
            {
                free(env->env_var[shlvl_index]);
                env->env_var[shlvl_index] = new_shlvl;
            }
            // free(env->env_var[i]);
            // env->env_var[i] = new_shlvl;
            return;
        }
        i++;
    }

    // 转换为数字并处理边界情况
    level = ft_atoi(shlvl_str);

    if (level < 0)
        level = 0;
    else if (level >= 2147483647)
        level = 1;
    else
        level++;

    // 创建新的SHLVL字符串
    new_value = ft_itoa(level);
    new_shlvl = ft_strjoin("SHLVL=", new_value);
    free(new_value);
    
    // 更新环境变量
    if (shlvl_index >= 0)
    {
        free(env->env_var[shlvl_index]);
        env->env_var[shlvl_index] = new_shlvl;
    }
}

void	init_env(t_env *env, char **envp)
{
	int	i;
	int	count;

	if (!envp)
	{
		env->env_var = NULL;
		env->exported = NULL;
		env->exit_status = 0;
		env->at_prompt = 0;
		return ;
	}
	i = 0;
	count = count_envp(envp);
	env->env_var = (char **)ft_malloc((count + 1) * sizeof(char *));
	while (envp[i])
	{
		env->env_var[i] = ft_strdup(envp[i]);
		i++;
	}
	env->env_var[i] = NULL;
	env->exported = NULL;
	env->exit_status = 0;
	env->at_prompt = 0;
}

/* printout the env variables */
void	print_env(t_env *env)
{
	int	i;

	i = 0;
	while (env && env->env_var && env->env_var[i])
	{
		printf("%s\n", env->env_var[i]);
		i++;
	}
}

/* free malloc for env */
void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env || !env->env_var)
		return ;
	while (env->env_var[i])
		free(env->env_var[i++]);
	free(env->env_var);
	if (env->exported)
		free(env->exported);
}
