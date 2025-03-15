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
    - else, return 1 as valid
*/
int	check_valid_name(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
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
			return ;
		}
	}
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
	char	*key;
	char	*value;

	if (!env)
		return ;
	if (args[1] == NULL)
		print_env(env);
	i = 1;
	while (args[i])
	{
		split_key_value(args[i], &key, &value);
		if (check_valid_name(key) == 0)
		{
			ft_putstr_fd(" not a valid identifier\n", 2);
			env->exit_status = 1;
			i++;
			continue ;
		}
		if (value == NULL)
			update_env(key, "''", env);
		else
			update_env(key, value, env);
		i++;
	}
}
/*
// // === test is_valid_name ===
// int main(int ac, char **av)
// {
//     (void)ac;
//     printf("%d\n", is_valid_name(av[1]));
// }
// // === test split_key_value ===
// int main(int ac, char **av)
// {
//     char *key;
//     char *value;

//     (void)ac;
//     split_key_value(av[1], &key, &value);
//     printf("key: %s, value: %s\n", key, value);
// }
// === test ft_export ===
int main(int ac, char **av, char **envp)
{
    t_env env;
    
    (void)ac;
    init_env(&env, envp);
    printf("env before export: \n\n");
    print_env(&env);

    ft_export(av, &env, envp);

    printf("env after export: \n\n");
    print_env(&env);
}*/
