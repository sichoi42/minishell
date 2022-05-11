#include "minishell.h"
#include <stdlib.h>

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

char	*ft_strdup(char *src)
{
	char	*dst;
	int		i;

	dst = malloc(ft_strlen(src) + 1);
	if (dst == NULL)
		return (NULL);
	i = 0;
	if (src)
	{
		while (src[i])
		{
			dst[i] = src[i];
			++i;
		}
	}
	dst[i] = 0;
	return (dst);
}

char	*ft_strndup(char *src, int len)
{
	char	*dst;
	int		i;

	dst = malloc(len + 1);
	if (dst == NULL)
		return (NULL);
	i = 0;
	if (src)
	{
		while (src[i] && i < len)
		{
			dst[i] = src[i];
			++i;
		}
	}
	dst[i] = 0;
	return (dst);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}
