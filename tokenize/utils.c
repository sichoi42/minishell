#include "../minishell.h"
#include "stdlib.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strndup(char *src, int len)
{
	char	*dst;
	int		i;
	int		n;

	n = ft_strlen(src);
	if (n < len)
		len = n;
	dst = malloc(len + 1);
	if (dst == NULL)
		return (NULL);
	i = 0;
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (dst);
}

char	*get_token_str(enum Token token)
{
	char	*s;

	if (token == T_WORD)
		s = "T_WORD";
	else if (token == T_QUOTE)
		s = "T_QUOTE";
	else if (token == T_DQUOTE)
		s = "T_DQUOTE";
	else if (token == T_PIPE)
		s = "T_PIPE";
	else if (token == T_DOLLAR)
		s = "T_DOLLAR";
	else if (token == T_STAR)
		s = "T_STAR";
	else if (token == T_RE_INPUT)
		s = "T_RE_INPUT";
	else if (token == T_RE_OUTPUT)
		s = "T_RE_OUTPUT";
	else if (token == T_RE_HEREDOC)
		s = "T_RE_HEREDOC";
	else if (token == T_RE_APPEND_OUTPUT)
		s = "T_RE_APPEND_OUTPUT";
	return (s);
}

char	*get_type_str(enum Type type)
{
	char	*s;

	if (type == ARGS)
		s = "ARGS";
	else if (type == PIPE)
		s = "PIPE";
	else if (type == REDIRECT)
		s = "REDIRECT";
	else if (type == STAR)
		s = "STAR";
	return (s);
}
