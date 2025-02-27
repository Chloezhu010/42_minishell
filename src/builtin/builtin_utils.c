#include "../incl/builtin.h"

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

/* add new_entry to the end of the existing env array
	- count the # of items in the existing env array
	- relloac for count + 2: new entry + null terminator
*/
void	add_env(char *key, char *value, t_env *env)
{
	int		count;
	char	**new_env;

	count = count_env(env);
	new_env = Realloc(env->env_var, (count + 2) * sizeof(char *));
	env->env_var = new_env;
	env->env_var[count] = create_env_entry(key, value);
	env->env_var[count + 1] = NULL;
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
	// if found, update the old entry with new entry
	i = 0;
	while (env->env_var[i])
	{
		if (ft_strncmp(env->env_var[i], key, key_len) == 0 && env->env_var[i][key_len] == '=')
		{
			free(env->env_var[i]);
			env->env_var[i] = new_entry;
			return ;
		}
		i++;
	}
	// if not found, add the new entry at the end
	add_env(key, value, env);
}
