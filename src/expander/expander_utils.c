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
char *handle_var_expansion(char *res, char **ptr)
{
    char *var_name;
    char *expanded;
    char *temp;

    var_name = extract_var_name(*ptr);
    expanded = expand_variable(var_name);
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