#include "../../incl/minishell.h"

/* global variables */
extern int g_exit_status;

/*	expand a single env_var
	- assumption:
		- the str should start with $, eg. $USER
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

/* extract the var name from a double quoted str
	- example
		- extract $USER from "hi $USER this is 42"
*/
char *extract_var_name(char *str)
{
	char *var_start;
	char *var_end;

	/* find the $ sign */
	var_start = ft_strchr(str, '$');
	if (!var_start)
		return (NULL);
	/* find the var_end */
	var_end = var_start + 1;
	while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
		var_end++;
	return (strndup(var_start, var_end - var_start)); //TODO replace
}

/* expand var in a str */
char *expand_var_instr(char *input)
{
	char *res;
	char *ptr = input;
	char *var_name;
	char *expanded;
	char *temp;

	res = ft_strdup("");
	while (*ptr)
	{
		if (*ptr == '$')
		{
			var_name = extract_var_name(ptr);
			expanded = expand_variable(var_name);
			if (expanded)
			{
				temp = ft_strjoin(res, expanded);
				free(res);
				res = temp;
				free(expanded);
			}
			ptr += ft_strlen(var_name) + 1;
			free(var_name);
		}
		else
		{
			char temp_char[2] = {*ptr, '\0'};
			temp = ft_strjoin(res, temp_char);
			free(res);
			res = temp;
			ptr++;
		}
	}
	return (res);
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
	char	*expanded_value;
    t_token *current;

    current = tokens;
	while (current)
	{
        /* skip single quoted string */
        if (current->type == TOKEN_SINGLE_QUOTE && current->value)
        {
            current = current->next;
            continue ;
        }
        /* handle double quoted string or normal words */
		else if (current->value && (current->type == TOKEN_DOUBLE_QUOTE || current->type == TOKEN_WORD))
		{
			expanded_value = expand_var_instr(current->value);
			// printf("debug: expanded name: %s\n", expanded_value); //debug
			/* add check for expanded */
            if (expanded_value)
            {
                free(current->value);
                current->value = expanded_value;
            }
		}
		current = current->next;
	}
}

// //===test extract_var_name ===
// int main()
// {
// 	char *str = "hi $USER_NAME bkdsk_dksekd";
// 	printf("extracted name: %s\n", extract_var_name(str));
// }