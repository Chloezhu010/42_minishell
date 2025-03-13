#include "../../incl/minishell.h"

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
char	*expand_variable(const char *str)
{
	const char	*env;
	int			exit_status = 1;

	if (!str || str[0] == '\0')
		return (ft_strdup(""));
	if (str[0] != '$')
		return (ft_strdup(str));
	if (ft_strcmp(str, "$?") == 0)
		return (ft_itoa(exit_status));
	env = getenv(str + 1);
	if (!env)
		return (ft_strdup(""));
	return (ft_strdup(env));
}

/* extract the var name from a double quoted str
	- use case
		- extract $USER from "hi $USER this is 42"
	- implementation
		- find the $ sign using ft_strchr
		- find the var_end
			- whenever it's alpha numberic or "_", move the ptr forward
*/
char *extract_var_name(char *str)
{
	char *var_start;
	char *var_end;

	var_start = ft_strchr(str, '$');
	if (!var_start)
		return (NULL);
	var_end = var_start + 1;
	while (*var_end)
	{
		if (*var_end && (*var_end == '?' || (*var_end >= '0' && *var_end <= '9')))
		{
			var_end++;
			break;
		}
		if ((*var_end >= 'A' && *var_end <= 'Z') || *var_end == '_')
		{
			while ((*var_end >= 'A' && *var_end <= 'Z') || *var_end == '_' || (*var_end >= '0' && *var_end <= '9'))
				var_end++;
			break;
		}
	}
	return (strndup(var_start, var_end - var_start)); //TODO replace
}

/* expand var in a str
	- set up an empty str res
	- loop through the input str
		- if encounter $
			- call handle_var_expansion
		- if not (a regular string)
			- call handle_regular_char
*/
char *expand_var_instr(char *input)
{
	char *res;
	char *ptr;

	ptr = input;
	res = ft_strdup("");
	while (*ptr)
	{
		if (*ptr == '$')
			res = handle_var_expansion(res, &ptr);
		else
		{
			res = handle_regular_char(res, *ptr);
			ptr++;
		}
	}
	return (res);
}

/*  expand var in the token array
	- skip single quoted string
	- handle double quoted string or normal string
		- expand var using expand_var_instr
		- if expand successfully
			- free current token value
			- update with the expanded value
		- move to the next token
*/
void	expand_tokens(t_token *tokens)
{
	char	*expanded_value;
    t_token *current;

    current = tokens;
	while (current)
	{
        if (current->type == TOKEN_SINGLE_QUOTE && current->value)
        {
            current = current->next;
            continue ;
		}
		else if (current->value && (current->type == TOKEN_DOUBLE_QUOTE || current->type == TOKEN_WORD))
		{
			expanded_value = expand_var_instr(current->value);
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