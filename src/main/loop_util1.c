/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 13:57:37 by auzou             #+#    #+#             */
/*   Updated: 2025/02/24 19:15:36 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

char	*extract_quoted(char *input, int *i, char quote)
{
	int		start;
	char	*quoted;

	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		printf("Error: Unclosed quote\n");
		return NULL;
	}
	quoted = strndup(&input[start], *i - start);
	(*i)++;
	return (quoted);
}

t_token	*tokenize(char *input)
{
	t_token *tokens = NULL;
	int i = 0;

	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (is_special_char(input[i]))
		{
			char op[3] = {0};
			op[0] = input[i];
			if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
				op[1] = input[++i];
			else if (input[i] == '&' && input[i + 1] == '&')
			{
				op[1] = '&';
				i++;
			}
			add_token(&tokens, create_token(op, get_token_type(op)));
			i++;
		}
		else if (input[i] == '"' || input[i] == '\'')
		{
			char quote = input[i++];
			char *quoted = extract_quoted(input, &i, quote);
			if (!quoted)
				return (NULL);
			add_token(&tokens, create_token(quoted, TOKEN_QUOTE));
			free(quoted);
		}
		else
		{
			int start = i;
			while (input[i] && !ft_isspace(input[i]) && !is_special_char(input[i]))
				i++;
			char *word = strndup(&input[start], i - start);
			add_token(&tokens, create_token(word, TOKEN_WORD));
			free(word);
		}
	}
	return (tokens);
}



char	*expand_variable(const char *str)
{
	// char		*status;
	const char	*env;

	if (str[0] != '$')
		return (strdup(str));
	// if (strcmp(str, "$?") == 0)
	// {
	// 	status = ft_itoa(g_exit)
	// 	return (status);
	// }
	env = getenv(str + 1);
	return env ? strdup(env) : strdup("");
}

void	expand_tokens(t_token *tokens)
{
	char	*expanded;

	while (tokens)
	{
		if (tokens->value[0] == '$')
		{
			expanded = expand_variable(tokens->value);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}

int	check_format_of_pipe(t_token *tokens)
{
	if (!tokens->next || tokens->next->type == TOKEN_PIPE)
	{
		printf("bash: syntax error near unexpected token '|'\n");
		return (0);
	}
	return (1);
}

int	check_format_of_redirect(t_token *tokens)
{
	if (!tokens->next)
		return (printf("bash: syntax error near unexpected token 'newline'\n"));
	else if (tokens->type == TOKEN_REDIRECT_IN
		&& tokens->next->type == TOKEN_REDIRECT_OUT)
		return (printf("bash: syntax error near unexpected token 'newline'\n"));
	else if (tokens->next->type == TOKEN_REDIRECT_IN
		|| tokens->next->type == TOKEN_REDIRECT_OUT
		|| tokens->next->type == TOKEN_REDIRECT_APPEND
		|| tokens->next->type == TOKEN_HEREDOC
		|| tokens->next->type == TOKEN_PIPE)
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			tokens->next->value);
		return (1);
	}
	else if (tokens->next->type != TOKEN_WORD)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		return (1);
	}
	return (0);
}

int	validate_tokens(t_token *tokens)
{
	if (!tokens)
		return (0);
	while (tokens)
	{
		if (tokens->type == TOKEN_OR)
		{
			printf("bash: syntax error near unexpected token '||'\n");
			return (0);
		}
		else if (tokens->type == TOKEN_PIPE && (!check_format_of_pipe(tokens)))
			return (0);
		else if ((tokens->type == TOKEN_REDIRECT_IN
				|| tokens->type == TOKEN_REDIRECT_OUT
				|| tokens->type == TOKEN_REDIRECT_APPEND
				|| tokens->type == TOKEN_HEREDOC)
			&& (check_format_of_redirect(tokens)))
		{
			return (0);
		}
		else if (tokens->type == TOKEN_AND || tokens->type == TOKEN_AND_SINGLE)
		{
			if (!tokens->next || tokens->next->type == TOKEN_AND || tokens->next->type == TOKEN_PIPE)
			{
				printf("bash: syntax error near unexpected token `%s'\n", tokens->value);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}


int	ft_isspace(char a)
{
	if (a == ' ' || a == '\t' || a == '\n')
		return (1);
	return (0);
}

t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	get_token_type(char *s)
{
	if (strcmp(s, "|") == 0)
		return (TOKEN_PIPE);
	if (strcmp(s, "||") == 0)
		return (TOKEN_OR);
	if (strcmp(s, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (strcmp(s, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (strcmp(s, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	if (strcmp(s, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (strcmp(s, "&&") == 0)
		return (TOKEN_AND);
	if (strcmp(s, "&") == 0)
		return (TOKEN_AND_SINGLE);
	return (TOKEN_WORD);
}


void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		free(tokens->value);
		tokens = tokens->next;
		free(tmp);
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds;
		i = 0;
		while (cmds->args && cmds->args[i])
			free(cmds->args[i++]);
		free(cmds->args);
		free(cmds->infile);
		free(cmds->outfile);
		cmds = cmds->next;
		free(tmp);
	}
}



t_cmd	*create_new_cmd(void)
{
	t_cmd	*cmd = calloc(1, sizeof(t_cmd));

	if (!cmd)
		return NULL;
	cmd->args = calloc(64, sizeof(char *));
	return cmd;
}

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *cmd_head = NULL;
    t_cmd *current_cmd = NULL;
    int i;

    if (!validate_tokens(tokens))
        return NULL;

    while (tokens)
    {
        if (!current_cmd)
        {
            current_cmd = create_new_cmd(); // 创建一个新的命令
            if (!cmd_head)
                cmd_head = current_cmd;
        }

        if (tokens->type == TOKEN_WORD
			|| tokens->type == TOKEN_QUOTE
			|| tokens->type == TOKEN_COMMAND)
        {
            i = 0;
            while (current_cmd->args[i]) // 依次找到当前命令中空闲的位置
                i++;
            current_cmd->args[i] = strdup(tokens->value); // 将当前词添加到命令的参数中
        }
        else if (tokens->type == TOKEN_REDIRECT_IN
            || tokens->type == TOKEN_REDIRECT_OUT
            || tokens->type == TOKEN_REDIRECT_APPEND)
        {
            tokens = tokens->next;
            if (tokens)
            {
                if (tokens->type == TOKEN_REDIRECT_OUT)
                {
                    current_cmd->outfile = strdup(tokens->value);
                    current_cmd->append = 0;
                }
                else if (tokens->type == TOKEN_REDIRECT_APPEND)
                {
                    current_cmd->outfile = strdup(tokens->value);
                    current_cmd->append = 1;
                }
                else
                    current_cmd->infile = strdup(tokens->value);
            }
        }
        else if (tokens->type == TOKEN_PIPE) // 如果遇到管道符号，创建新的命令
        {
            if (tokens->next && (tokens->next->type == TOKEN_COMMAND || tokens->next->type == TOKEN_QUOTE))
            {
                current_cmd->next = create_new_cmd(); // 创建新的命令块
                current_cmd = current_cmd->next; // 设置当前命令为新的命令块
            }
        }

        tokens = tokens->next;
    }

    return cmd_head;
}



// void	print_cmds(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (cmd)
// 	{
// 		printf("\nCommand:\n");
// 		while (cmd->args && cmd->args[i])
// 		{
// 			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
// 			i++;
// 		}
// 		if (cmd->infile)
// 			printf("  Input Redirect: %s\n", cmd->infile);
// 		if (cmd->outfile)
// 			printf("  Output Redirect: %s (%s)\n",
// 				cmd->outfile, cmd->append ? "append" : "overwrite");
// 		cmd = cmd->next;
// 		i = 0;
// 	}
// }

// void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		printf("Token: %-10s | Type: %d\n", tokens->value, tokens->type);
// 		tokens = tokens->next;
// 	}
// }

void	check_format_command(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if ((strcmp(tokens->value, "cd") == 0)
				|| (strcmp(tokens->value, "ls") == 0)
				|| (strcmp(tokens->value, "cp") == 0)
				|| (strcmp(tokens->value, "mv") == 0)
				|| (strcmp(tokens->value, "rm") == 0)
				|| (strcmp(tokens->value, "mkdir") == 0)
				|| (strcmp(tokens->value, "rmdir") == 0)
				|| (strcmp(tokens->value, "man") == 0)
				|| (strcmp(tokens->value, "more") == 0)
				|| (strcmp(tokens->value, "cat") == 0)
				|| (strcmp(tokens->value, "echo") == 0)
				|| (strcmp(tokens->value, "grep") == 0)
				|| (strcmp(tokens->value, "pwd") == 0)
				|| (strcmp(tokens->value, "export") == 0)
				|| (strcmp(tokens->value, "unset") == 0)
				|| (strcmp(tokens->value, "env") == 0)
				|| (strcmp(tokens->value, "exit") == 0))
				tokens->type = TOKEN_COMMAND;
		}
		tokens = tokens->next;
	}
}

// int	main(void)
// {
// 	char	*input;
// 	t_token	*tokens;
// 	t_cmd	*cmds;

// 	while (1){
// 		input = readline("$>");
// 		tokens = tokenize(input);
// 		if (tokens != NULL)
// 		{
// 			expand_tokens(tokens);
// 			check_format_command(tokens);
// 			print_tokens(tokens);
// 			cmds = parse_tokens(tokens);
// 			if (cmds)
// 				print_cmds(cmds);
// 		}
// 		if (tokens != NULL)
// 			free_tokens(tokens);
// 		if (cmds != NULL)
// 			free_cmds(cmds);
// 	}
// 	return (0);
// }
