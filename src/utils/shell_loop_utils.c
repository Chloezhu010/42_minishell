/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:59:44 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 17:59:45 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	*ft_malloc(size_t size)
{
	void	*ptr;

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

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 || *s2) && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void	process_heredocs(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd_temp;
	int		fd;

	cmd_temp = cmds;
	while (cmd_temp)
	{
		if (cmd_temp->heredoc && cmd_temp->delimiter)
		{
			fd = handle_heredoc(cmd_temp->delimiter, env);
			if (fd != -1)
				cmd_temp->fd_in = fd;
		}
		cmd_temp = cmd_temp->next;
	}
}

void	execute_commands(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd_temp;

	if (!cmds)
		return ;
	if (cmds->next)
	{
		cmd_temp = cmds;
		while (cmd_temp)
		{
			cmd_temp->in_pipe = 1;
			cmd_temp = cmd_temp->next;
		}
		execute_pipeline(cmds, env);
	}
	else
	{
		cmds->in_pipe = 0;
		execute_shell(cmds, env);
	}
}

char	*get_command_line(void)
{
	char	*line;

	line = read_line();
	if (line == NULL)
	{
		printf("exit\n");
		return (NULL);
	}
	return (line);
}
