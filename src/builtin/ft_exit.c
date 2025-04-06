/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:56:07 by czhu              #+#    #+#             */
/*   Updated: 2025/03/25 17:10:29 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* exit with no option
	requirement
	- need to exit with numeric arg or no arg
	- take only 1 arg
		- 0: success
		- non zero: error
*/
static int	is_valid_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-')
		i = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_too_many_args(char **args, t_env *env)
{
	ft_putstr_fd(" too many arguments\n", 2);
	if (!is_valid_numeric_arg(args[1]))
		exit_status(env, 2);
	else
		exit_status(env, 1);
}

static int	calculate_char_sum(char *str)
{
	int	i;
	int	result;

	result = 0;
	i = 0;
	if (str[0] == '-')
		i = 1;
	while (str[i])
		result += (int)str[i++];
	return (result);
}

static void	handle_exit_arg(char *arg, t_env *env)
{
	int	exit_code;
	int	char_sum;

	if (is_digit(arg) == 0)
	{
		ft_putstr_fd(" numeric argument required\n", 2);
		exit_status(env, 2);
		env->exit_requested = 1;
		return ;
	}
	char_sum = calculate_char_sum(arg);
	if ((arg[0] == '-' && char_sum > 1001)
		|| (arg[0] != '-' && char_sum > 1000))
	{
		ft_putstr_fd("bash: exit: : numeric argument required\n", 2);
		exit_code = 2;
	}
	else
		exit_code = (int)ft_atoi(arg);
	exit_status(env, exit_code);
	env->exit_requested = 1;
}

void	ft_exit(char **args, t_env *env)
{
	int	fd;

	ft_putstr_fd("exit\n", 1);
	if (args[1] != NULL && args[2] != NULL)
	{
		handle_too_many_args(args, env);
		return ;
	}
	if (args[1] != NULL)
		handle_exit_arg(args[1], env);
	else
	{
		exit_status(env, 0);
		env->exit_requested = 1;
	}
	fd = 3;
	while (fd < 10)
	{
		close(fd);
		fd++;
	}
}
