#include "../incl/builtin.h"

/* init the env from envp
    - count the env variables
    - malloc for env variables
    - copy env variables to my own struct
    - null terminated
*/
void	init_env(t_env *env, char **envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!envp)
	{
		env->env_var = NULL;
		return ;
	}
	while (envp[count])
		count++;
	env->env_var = (char **)malloc((count + 1) * sizeof(char *));
	if (!env->env_var)
	{
		perror("malloc");
		exit (1);
	}
	while (envp[i])
	{
		env->env_var[i] = ft_strdup(envp[i]);
		i++;
	}
	env->env_var[i] = NULL;
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
	{
		free(env->env_var[i]);
		i++;
	}
	free(env->env_var);
}

/* create a new entry of a key-value pair in env array
    - malloc for the new entry
    - copy the key, "=", new value, "\0" to the env array
    - return the new_entry
*/
char	*create_env_entry(char *key, char *value)
{
	int		key_len;
	int		value_len;
	char	*new_entry;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_entry = (char *)malloc(key_len + value_len + 2);
	if (!new_entry)
		perror("malloc");
	ft_memcpy(new_entry, key, key_len);
	new_entry[key_len] = '=';
	ft_memcpy(new_entry + key_len + 1, value, value_len);
	new_entry[key_len + value_len + 1] = '\0';
	return (new_entry);
}

/* update the env var after calling ft_cd
    - malloc for the new entry
    - loop through the env array
        - if find key matches, update the old entry with new entry
        - if not, add the new entry at the end, null terminated
*/
void	update_env(char *key, char *value, t_env *env)
{
	int		i;
	int		key_len;
	char	*new_entry;

	key_len = ft_strlen(key);
	new_entry = create_env_entry(key, value);
	i = 0;
	while (env->env_var[i])
	{
		if (ft_strncmp(env->env_var[i], key, key_len) == 0 && env->env_var[i][key_len] == '=')
		{
			free(env->env_var[i]);
			env->env_var[i] = new_entry;
		}
        env->env_var[i] = new_entry;
        env->env_var[i + 1] = NULL;
		i++;
	}
}