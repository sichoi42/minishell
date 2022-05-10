#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

void	print_token_str(enum e_token token)
{
	if (token & T_WORD)
		printf("%s, ", "T_WORD");
	if (token & T_PIPE)
		printf("%s, ", "T_PIPE");
	if (token & T_STAR)
		printf("%s, ", "T_STAR");
	if (token & T_RE_INPUT)
		printf("%s, ", "T_RE_INPUT");
	if (token & T_RE_OUTPUT)
		printf("%s, ", "T_RE_OUTPUT");
	if (token & T_RE_HEREDOC)
		printf("%s, ", "T_RE_HEREDOC");
	if (token & T_RE_APPEND_OUTPUT)
		printf("%s, ", "T_RE_APPEND_OUTPUT");
	if (token & T_EXIT_CODE)
		printf("%s, ", "T_EXIT_CODE");
	printf("\n");
}

void	print_type_str(enum e_type type)
{
	if (type & ARGS)
		printf("%s, ", "ARGS");
	if (type & PIPE)
		printf("%s, ", "PIPE");
	if (type & REDIRECT)
		printf("%s, ", "REDIRECT");
	if (type & STAR)
		printf("%s, ", "STAR");
	printf("\n");
}

void	print_token_list(t_token *t)
{
	t_token	*p;

	if (t)
	{
		p = t->next;
		while (p)
		{
			printf("%s\n", p->s);
			print_token_str(p->token);
			print_type_str(p->type);
			printf("-----------------\n");
			p = p->next;
		}
	}
}

void	free_token(t_token *t)
{
	t_token	*p;
	t_token	*tmp;

	if (t)
	{
		p = t;
		while (p)
		{
			if (p->next)
			{
				tmp = p->next;
				free(p->s);
				free(p);
				p = tmp;
			}
			else
			{
				free(p->s);
				free(p);
			}
			p = p->next;
		}
	}
}

// s1과 s2의 최대 n개를 연결하여 반환하는 함수.(메모리 복제가 발생하고, s1은 free하지만 s2는 free하지 않음.)

char	*ft_strnjoin(char *s1, char *s2, int len)
{
	char	*str;
	int		i;
	int		j;
	int		n;

	if (s1 == NULL && s2 != NULL)
	{
		return (ft_strndup(s2, len));
	}
	if (s1 != NULL && s2 == NULL)
	{
		free(s1);
		return (ft_strndup(s1, len));
	}
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (str == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	n = i;
	i = 0;
	j = 0;
	while (s2[j] && j < len)
	{
		if (s2[j] != '\\' || (j > 0 && s2[j - 1] == '\\' && s2[j] == '\\'))
		{
			str[n + i] = s2[j];
			++i;
		}
		++j;
	}
	str[n + i] = 0;
	free(s1);
	return (str);
}

char	*ft_strchr(char *s, char c)
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		++s;
	}
	return (NULL);
}

char	*ft_strchr_ig_blsh(char *s, char *c, char **end)
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*c == '\\')
			if (*(c + 1) && *s == *(c + 1))
				++(*end);
		if (*s == *c)
			return ((char *)s);
		++s;
	}
	return (NULL);
}
