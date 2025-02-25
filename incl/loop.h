#ifndef LOOP_H
# define LOOP_H

# include "../incl/minishell.h"

# define TOKEN_WORD 0
# define TOKEN_PIPE 1
# define TOKEN_OR 10
# define TOKEN_REDIRECT_IN 2
# define TOKEN_REDIRECT_OUT 3
# define TOKEN_REDIRECT_APPEND 4
# define TOKEN_HEREDOC 5
# define TOKEN_QUOTE 6
# define DIRECTORY '/'
# define TOKEN_AND 7
# define TOKEN_AND_SINGLE 8
# define TOKEN_COMMAND 9


typedef struct s_token {
    char			*value;
    int				type;
    struct s_token	*next;
}				t_token;

typedef struct s_cmd {
    char			**args;
    char			*infile;
    char			*outfile;
    int				append;
    struct s_cmd	*next;
}				t_cmd;

char	*extract_quoted(char *input, int *i, char quote);
t_token *tokenize(char *input);
char	*expand_variable(const char *str);
void	expand_tokens(t_token *tokens);
int	check_format_of_pipe(t_token *tokens);
int	check_format_of_redirect(t_token *tokens);
int	validate_tokens(t_token *tokens);
int	ft_isspace(char a);
t_token	*create_token(char *value, int type);
void	add_token(t_token **head, t_token *new_token);
int	is_special_char(char c);


int	get_token_type(char *s);
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);
t_cmd	*create_new_cmd(void);
t_cmd *parse_tokens(t_token *tokens);
// void	print_cmds(t_cmd *cmd);
// void	print_tokens(t_token *tokens);
void	check_format_command(t_token *tokens);

#endif