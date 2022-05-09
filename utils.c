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

char	*ft_strchr(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			if (*(s - 1) != '\\')
				return ((char *)s);
		++s;
	}
	return (NULL);
}

char	*ft_strndup(char *src, int len)
{
	char	*dst;
	int		i;
	int		j;
	int		n;

	n = ft_strlen(src);
	if (n < len)
		len = n;
	dst = malloc(len + 1);
	if (dst == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (src[j] && j < len)
	{
		if (src[j] != '\\')
		{
			dst[i] = src[j];
			++i;
		}
		++j;
	}
	dst[i] = 0;
	return (dst);
}

// int	is_space(char c)
// {
// 	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
// 		return (1);
// 	return (0);
// }

// char	*ft_strchr(char *s, int c)
// {
// 	while (*s && *s != (char)c)
// 		++s;
// 	if (*s == (char)c)
// 		return ((char *)s);
// 	return (NULL);
// }
