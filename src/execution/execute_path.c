/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:18:19 by czhu              #+#    #+#             */
/*   Updated: 2025/03/02 13:18:53 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/execution.h"

/* helper function to get PATH */
char	*get_path(t_env *env)
{
	int		i;
	int		path_len;

	if (!env || !env->env_var)
		return (NULL);
	i = 0;
	while (env->env_var[i])
	{
		path_len = 5;
		if (ft_strncmp(env->env_var[i], "PATH=", path_len) == 0)
			return (env->env_var[i] + path_len);
		i++;
	}
	return (NULL);
}

/* find the executable path for execve
    - find the PATH in env var
    - split path using ":" to get dir array
    - loop through the dir array + "/" + cmd to find the executable path
        - check if full_path is executable
        - if yes, return full_path
        - if not, free all, i++
    - if no executable path found, cleanup dir array and return NULL
*/
char	*find_path(char *cmd, t_env *env)
{
	char	*path;
	char	**dir;
	char	*tmp;
	char	*full_path;
	int		i;

	path = get_path(env);
	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	if (!dir)
		return (NULL);
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
	return (NULL);
}

/* get the full path for an external cmd 
	- if the cmd name contains absolute path, then copy the path
	- if not, find_path
*/
char	*get_cmd_path(t_cmd *cmd, t_env *env)
{
	char	*path;

	path = NULL;
	if (cmd->args && cmd->args[0])
	{
		if (ft_strchr(cmd->args[0], '/'))
			path = ft_strdup(cmd->args[0]);
		else
		{
			path = find_path(cmd->args[0], env);
			if (!path)
				return (NULL);
		}
	}
	return (path);
}
// // ==== test find_path ===
// int main(int ac, char **av)
// {
//     char *full_path;

//     (void)ac;
//     full_path = find_path(av[1]);
//     printf("%s\n", full_path);
// }
