/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:56:07 by czhu              #+#    #+#             */
/*   Updated: 2025/02/16 16:56:28 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* echo with -n option: print argus
	- start from args[1], skip "echo"
	- set new_line as default 1
    - handle -n option
		- if yes, set new_line to 0
	- normal case: print out the args
    - print out the newline in the end
*/
static int	handle_n_option(char *arg)
{
	int	x;

	x = 2;
	if (ft_strncmp(arg, "-n", 2) == 0 && ft_strlen(arg) == 2)
		return (0);
	else if (ft_strlen(arg) > 2)
	{
		if (ft_strncmp(arg, "-n", 2) == 0)
		{
			while (arg[x] == 'n')
				x++;
			if (x == (int)ft_strlen(arg))
				return (0);
		}
	}
	return (1);
}

void	ft_echo(char **args, t_env *env)
{
	int	i;
	int	new_line;

	(void)env;
	i = 1;
	new_line = 1;
	while (args[i] && args[i][0] == '-')
	{
		if (handle_n_option(args[i]) == 0)
			new_line = 0;
		else
			break ;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (new_line == 1)
		printf("\n");
}
