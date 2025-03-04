#include "../../incl/loop.h"

/* global variables */
extern int g_exit_status;

/*	replace the $param with env_var
	- if str is null, empty or space
		- return ""
	- if no $, copy the str as it is
	- if contains $
		- if "$?", return the exit value
		- if "$" + other cases
			- skip the $ char
			- search the parameter in env_var
*/
char	*expand_variable(const char *str)
{
	const char	*env;

	if (!str || str[0] == '\0')
		return (ft_strdup(""));
	if (str[0] != '$')
		return (ft_strdup(str));
	if (ft_strcmp(str, "$?") == 0)
		return (ft_itoa(g_exit_status));
	env = getenv(str + 1);
	if (!env)
		return (ft_strdup(""));
	return (ft_strdup(env));
}

/*  iterate through all args if they need expand
	- handle single quote case
    - check if token has $
	- if yes
		- call expand_variable()
		- replace the token value with env_var
	- if no
		- keep the original value
*/
void	expand_tokens(t_token *tokens)
{
	char	*expanded;
    t_token *current;

    current = tokens;
	while (current)
	{
        /* skip single quoted string */
        if (current->type == TOKEN_SINGLE_QUOTE && current->value)// TODO, need to change to quote_single
        {
            current = current->next;
            continue ;
        }
        /* add null check */
		else if (current->value && current->value[0] == '$')
		{
			expanded = expand_variable(current->value);
            /* add check for expanded */
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
            }
		}
		current = current->next;
	}
}