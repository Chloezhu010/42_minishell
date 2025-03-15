#include "../../incl/minishell.h"

/* helper function to handle regular char in expand_var_instr
    - create a temp str with current_char + \0
    - append the char to the res string
    - free the old res string
    - return the new temp string
*/
char *handle_regular_char(char *res, char current_char)
{
    char str[2];
    char *temp;

    str[0] = current_char;
    str[1] = '\0';
    temp = ft_strjoin(res, str);
    free(res);
    return (temp);
}

/*	expand a single env_var
	- use case
		- expand $USER to user_name in env
	- implementation
	- if str is null, empty or space
		- return ""
	- if no $, copy the str as it is
	- if contains $
		- if "$?", return the exit value
		- if "$" + other cases
			- skip the $ char
			- search the parameter in env_var
*/


static char	*expand_variable(const char *str, t_env *enve)
{
	const char	*env;

	if (!str || str[0] == '\0')
		return (ft_strdup(""));
	if (str[0] == '$' && str[1] == '\0')
		return (ft_strdup("$"));
	if (str[0] != '$')
		return (ft_strdup(str));
	if (ft_strcmp(str, "$?") == 0)
		return (ft_itoa(enve->exit_status));
	env = getenv(str + 1);
	if (!env)
		return (ft_strdup(""));
	return (ft_strdup(env));
}

/* helper function to handle var expansion in expand_var_instr
    - extract the var name
    - expand the var name
    - if the varis expanded successfully
        - append the expanded value to the res str
        - free the old res str and update with new one
        - free the expanded value
    - move the ptr pass the var name
    - free the extracted var name
*/
char *handle_var_expansion(char *res, char **ptr, t_env *env)
{
    char *var_name;
    char *expanded;
    char *temp;

    var_name = extract_var_name(*ptr);
    expanded = expand_variable(var_name, env);
    if (expanded)
    {
        temp = ft_strjoin(res, expanded);
        free(res);
        res = temp;
        free(expanded);
    }
    *ptr += ft_strlen(var_name);
    free(var_name);
    return (res);
}