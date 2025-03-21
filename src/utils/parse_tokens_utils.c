#include "../../incl/loop.h"
#include "../../incl/minishell.h"

t_cmd	*create_new_command(t_cmd **cmd_head)
{
	t_cmd	*new_cmd;

	new_cmd = create_new_cmd();
	if (!*cmd_head)
		*cmd_head = new_cmd;
	return (new_cmd);
}

void	process_token_word(t_cmd *current_cmd, t_token *tokens)
{
	int	i;

	i = 0;
	if ((i == 0 || !tokens->next || (tokens->next->type != TOKEN_REDIRECT_IN
				&& tokens->next->type != TOKEN_REDIRECT_OUT
				&& tokens->next->type != TOKEN_REDIRECT_APPEND))
		&& tokens->value[0] != '\0')
	{
		while (current_cmd->args[i])
			i++;
		current_cmd->args[i] = strdup(tokens->value);
	}
}

void	process_redirection(t_cmd *current_cmd, t_token **tokens, int type)
{
	*tokens = (*tokens)->next;
	if (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_DOUBLE_QUOTE
			|| (*tokens)->type == TOKEN_SINGLE_QUOTE))
		add_redir(current_cmd, (*tokens)->value, type);
}

void	process_heredoc(t_cmd *current_cmd, t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_DOUBLE_QUOTE
			|| (*tokens)->type == TOKEN_SINGLE_QUOTE))
	{
		current_cmd->heredoc = 1;
		current_cmd->delimiter = ft_strdup((*tokens)->value);
	}
}
