/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:04:12 by czhu              #+#    #+#             */
/*   Updated: 2025/03/12 15:05:12 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* check if the name is valid
    - if name is null or empty, return 0
    - first letter need to start with alpha or '_'
    - loop through the string
        - must only contain alphanumeric or '_'
        - if invalid, return 0
		- if key start with '-', return 2
    - else, return 1 as valid
*/
int	check_valid_name(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	if (key[0] == '-')
		return (2);
	if (!((key[0] >= 'a' && key[0] <= 'z')
			|| (key[0] >= 'A' && key[0] <= 'Z')
			|| (key[0] == '_')))
		return (0);
	i = 1;
	while (key[i])
	{
		if (! (ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/* split input into key & value
    - find the position of '='
    - if no '=' found
        - treat the entire string as key
    - else
        - malloc for key
        - extract the key part (before =), ft_strlcpy is null terminated
        - extract the value part (after =)
*/
void	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal == NULL)
	{
		*key = ft_strdup(arg);
		if (!*key)
			return ;
		*value = NULL;
	}
	else
	{
		*key = malloc(equal - arg + 1);
		if (!*key)
			return ;
		ft_strlcpy(*key, arg, equal - arg + 1);
		*value = ft_strdup(equal + 1);
		if (!*value)
		{
			free(*key);
			*key = NULL;
			return ;
		}
	}
}

/* helper function for process_single_export_arg */
static int	validate_export_key(char *key, t_env *env)
{
	if (!key || check_valid_name(key) == 0)
	{
		ft_putstr_fd(" not a valid identifier\n", 2);
		exit_status(env, 1);
		return (0);
	}
	if (!key || check_valid_name(key) == 2)
	{
		ft_putstr_fd(" not a valid identifier\n", 2);
		exit_status(env, 2);
		return (0);
	}
	return (1);
}

/* helper function for ft_export */
static void	process_single_export_arg(char *arg, t_env *env)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	split_key_value(arg, &key, &value);
	if (!validate_export_key(key, env))
	{
		free(key);
		free(value);
		return ;
	}
	if (value == NULL)
		update_env(key, "''", env);
	else
		update_env(key, value, env);
	free(key);
	free(value);
}

/* export with no option 
    export function
    - set a new env var
    - update an existing env var
    - list all exported env var (if no args provided)
    - make them available to child processes

    implementation of ft_export
    - check init env
    - input control
        - if no args
            - print_env
    - loop through the args
        - check if args has invalid char
            - if yes, print "export: not a valid identifier\n"
            - continue
        - split key & value, based on "="
        - if value = null
            - if key doesn't exit in env
                - update "key=" to the array
        - else
            - udpate they key & value to the array
*/
void	ft_export(char **args, t_env *env)
{
	int		i;

	if (!env)
		return ;
	if (args[1] == NULL)
	{
		print_env(env);
		return ;
	}
	i = 1;
	while (args[i])
	{
		process_single_export_arg(args[i], env);
		i++;
	}
}
