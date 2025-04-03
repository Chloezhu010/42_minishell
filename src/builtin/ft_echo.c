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
void	ft_echo(char **args, t_env *env)
{
	int	i;
	int	new_line;
	int	x;

	(void)env;
	i = 1;
	new_line = 1;
	while (args[i] && args[i][0] == '-')
	{
		if ((ft_strncmp(args[i], "-n", 2) == 0) && (ft_strlen(args[i]) == 2))
			new_line = 0;
		else if (ft_strlen(args[i]) > 2)
		{
			x = 2;
			while ((ft_strncmp(args[i], "-n", 2) == 0) && (args[i][x]))
			{
				if (args[i][x] == 'n')
					x++;
				else
					break ;
			}
			if (x == (int)ft_strlen(args[i]))
				new_line = 0;
		}
		else
			break ;
		if (x != (int)ft_strlen(args[i]))
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
