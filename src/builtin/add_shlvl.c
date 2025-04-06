/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_shlvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 02:46:52 by auzou             #+#    #+#             */
/*   Updated: 2025/04/06 02:53:21 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

static void	create_new_shlvl(t_env *env)
{
	int		i;
	char	*new_value;
	char	**new_env;

	i = 0;
	while (env->env_var && env->env_var[i])
		i++;
	new_value = ft_strdup("SHLVL=1");
	new_env = (char **)ft_malloc((i + 2) * sizeof(char *));
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
}

static int	find_shlvl(t_env *env)
{
	int		i;

	i = 0;
	while (env->env_var && env->env_var[i])
	{
		if (ft_strncmp(env->env_var[i], "SHLVL=", 6) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	update_shlvl(t_env *env, int shlvl_index)
{
	int		level;
	char	*shlvl_str;
	char	*new_value;
	char	*new_shlvl;

	shlvl_str = env->env_var[shlvl_index] + 6;
	if (!is_valid_number(shlvl_str))
	{
		new_shlvl = ft_strdup("SHLVL=1");
		free(env->env_var[shlvl_index]);
		env->env_var[shlvl_index] = new_shlvl;
		return ;
	}
	level = ft_atoi(shlvl_str);
	if (level < 0)
		level = 0;
	else if (level >= 2147483647)
		level = 1;
	else
		level++;
	new_value = ft_itoa(level);
	new_shlvl = ft_strjoin("SHLVL=", new_value);
	free(new_value);
	free(env->env_var[shlvl_index]);
	env->env_var[shlvl_index] = new_shlvl;
}

void	add_shlvl(t_env *env)
{
	int	shlvl_index;

	shlvl_index = find_shlvl(env);
	if (shlvl_index == -1)
		create_new_shlvl(env);
	else
		update_shlvl(env, shlvl_index);
}
