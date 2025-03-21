#include "../../incl/loop.h"

char	*extract_single_quoted(char *input, int *i, char quote)
{
	int		start;
	char	*quoted;

	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		printf("quote need be close\n");
		return (NULL);
	}
	if (*i - start <= 0)
		return (strdup(""));
	quoted = strndup(&input[start], *i - start);
	(*i)++;
	return (quoted);
}

char	*process_diff_quote(char *input, int *i, char quote, char *current)
{
	int		start;
	char	*segment;
	char	*result;

	start = *i;
	while (input[*i] && quote != input[*i])
		(*i)++;
	if (!input[*i])
	{
		printf("quote need be close\n");
		free(current);
		return (NULL);
	}
	segment = strndup(&input[start], *i - start);
	result = ft_strjoin(current, segment);
	free(segment);
	free(current);
	(*i)++;
	return (result);
}

char	*handle_consecutive_quote(char *input,
	int *i, char quote, char *current)
{
	++(*i);
	if (input[*i] && quote != input[*i])
		return (process_diff_quote(input, i, quote, current));
	else if (!input[*i])
	{
		printf("quote need be close\n");
		free(current);
		return (NULL);
	}
	return (current);
}

char	*process_additional_quotes(char *input, int *i, char *quoted)
{
	char	quote;
	char	*result;

	result = quoted;
	while (input[*i] && is_quote(input[*i]))
	{
		quote = input[*i];
		result = handle_consecutive_quote(input, i, quote, result);
		if (!result)
			return (NULL);
	}
	return (result);
}