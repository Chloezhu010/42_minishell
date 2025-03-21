#include "../../incl/loop.h"

static int	tokenize_util4_util1(char *input, int *start, char **word, int *i)
{
	char	quote;
	char	*new_part;
	char	*temp;

	while (input[*i] && is_quote(input[*i]))
	{
		quote = input[*i];
		*start = ++(*i);
		while (input[*i] && input[*i] != quote)
			(*i)++;
		if (!input[*i])
		{
			printf("quote need be closed");
			return (-1);
		}
		new_part = strndup(&input[*start], (*i) - (*start));
		temp = ft_strjoin(*word, new_part);
		free(new_part);
		free(*word);
		*word = temp;
		(*i)++;
	}
	return (0);
}

static int	tokenize_util4_append_word(char *input, int *i, char **word)
{
	int	start;

	if (is_quote(input[*i]))
	{
		start = *i;
		if (tokenize_util4_util1(input, &start, word, i) == -1)
			return (-1);
	}
	else
	{
		start = *i;
		while (input[*i] && !ft_isspace(input[*i]) 
			&& !is_special_char(input[*i]) && !is_quote(input[*i]))
			(*i)++;
		*word = ft_strjoin(*word, strndup(&input[start], (*i) - start));
	}
	return (0);
}

static int	tokenize_util4_parse_word(char *input, int *i, char **word)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_special_char(input[*i]) && !is_quote(input[*i]))
		(*i)++;
	if (*i > start)
		*word = strndup(&input[start], (*i) - start);
	else
		return (0);
	while (input[*i] && (!ft_isspace(input[*i]) && !is_special_char(input[*i])))
	{
		if (tokenize_util4_append_word(input, i, word) == -1)
			return (-1);
	}
	return (1);
}

void	tokenize_util4(t_token **tokens, int *i, char *input)
{
	char	*word;

	word = NULL;
	if (tokenize_util4_parse_word(input, i, &word) == 1)
	{
		add_token(tokens, create_token(word, TOKEN_WORD));
		free(word);
	}
	if (input[*i])
		(*i)++;
}