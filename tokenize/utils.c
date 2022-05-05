#include "utils.h"
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

char	*get_token_str(int token)
{
	char	*s;

	if (token == T_WORD)
		s = "WORD";
	else if (token == T_QUOTE)
		s = "QUOTE";
	else if (token == T_DQUOTE)
		s = "DQUOTE";
	else if (token == T_PIPE)
		s = "PIPE";
	else if (token == T_DOLLAR)
		s = "DOLLAR";
	else if (token == T_STAR)
		s = "STAR";
	else if (token == T_RE_INPUT)
		s = "RE_INPUT";
	else if (token == T_RE_OUTPUT)
		s = "RE_OUTPUT";
	else if (token == T_RE_HEREDOC)
		s = "RE_HEREDOC";
	else if (token == T_RE_APPEND_OUTPUT)
		s = "RE_APPEND_OUTPUT";
	return (s);
}
