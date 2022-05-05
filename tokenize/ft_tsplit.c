#include "utils.h"
#include <stdlib.h>

int	get_quote_token(char *s, int *i, int *sq_cnt, int *dq_cnt) // single인지 double quote인지에 따라 해당 토큰을 반환.
{
	int	token;

	if (s[*i] == T_QUOTE)
	{
		token = T_QUOTE;
		++(*sq_cnt);
	}
	else
	{
		token = T_DQUOTE;
		++(*dq_cnt);
	}
	return (token);
}

t_token	*split_quote(char *s, int *i, int *sq_cnt, int *dq_cnt) // line중 single quote와 double quote로 감싸진 word를 생성.
{
	int			token;
	int			j;
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->token = get_quote_token(s, i, sq_cnt, dq_cnt);
	++(*i);
	j = *i;
	while (*(s + *i) && *(s + *i) != new->token)
		++(*i);
	if (*(s + *i) == new->token)
	{
		if (new->token == T_QUOTE)
			++(*sq_cnt);
		else
			++(*dq_cnt);
		++(*i);
	}
	new->s = ft_strndup(s + j, *i - j - 1);
	if (new->s == NULL)
		return (NULL);
	return (new);
}

t_token	*split_one_elem(char *s, int *i) // line중 한글자로 이루어진 token(|, $, *)을 생성.
{
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	if (s[*i] == T_PIPE)
		new->token = T_PIPE;
	else if (s[*i] == T_DOLLAR)
		new->token = T_DOLLAR;
	else
		new->token = T_STAR;
	new->s = ft_strndup(&s[*i], 1);
	if(new->s == NULL)
		return (NULL);
	++(*i);
	return (new);
}

int	get_redirect_token(char *s, int *i) // 어떤 리다이렉션인지에 따라 해당 토큰을 반환.
{
	int	token;

	if (s[*i] == '>')
	{
		if (s[(*i) + 1] && s[(*i) + 1] == '>')
		{
			token = T_RE_APPEND_OUTPUT;
			++(*i);
		}
		else
			token = T_RE_OUTPUT;
	}
	else
	{
		if (s[(*i) + 1] && s[(*i) + 1] == '<')
		{
			token = T_RE_HEREDOC;
			++(*i);
		}
		else
			token = T_RE_INPUT;
	}
	++(*i);
	return (token);
}

t_token	*split_redirect(char *s, int *i) // line중 리다이렉션에 해당하는 토큰 생성.
{
	int		token;
	int		j;
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	token = get_redirect_token(s, i);
	new->token = token;
	while (s[*i] && s[*i] == ' ')
		++(*i);
	j = *i;
	while (s[*i] && s[*i] != ' ')
		++(*i);
	new->s = ft_strndup(s + j, *i - j);
	if(new->s == NULL)
		return (NULL);
	return (new);
}

t_token	*split_word(char *s, int *i) // line중 word에 해당하는 토큰 생성.
{
	int			j;
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->token = T_WORD;
	j = *i;
	while (*(s + *i) && *(s + *i) != ' ')
		++(*i);
	new->s = ft_strndup(s + j, *i - j);
	if (new->s == NULL)
		return (NULL);
	return (new);
}

int	ft_tsplit(t_token *t, char *s)
{
	int			i;
	int			sq_cnt;
	int			dq_cnt;
	t_token		*new;
	t_token		*p;

	sq_cnt = 0;
	dq_cnt = 0;
	p = t;
	i = 0;
	while (*(s + i))
	{
		while (*(s + i) && *(s + i) == ' ')
			++i;
		if (*(s + i) == T_QUOTE || *(s + i) == T_DQUOTE)
			new = split_quote(s, &i, &sq_cnt, &dq_cnt); // ', "
		else if (*(s + i) == T_PIPE || *(s + i) == T_DOLLAR || *(s + i) == T_STAR)
			new = split_one_elem(s, &i); // |, $, *
		else if (*(s + i) == '>' || *(s + i) == '<')
			new = split_redirect(s, &i); // >, <, >>, <<
		else
			new = split_word(s, &i);
		if (new == NULL)
			return (-1);
		p->next = new;
		p = p->next;
	}
	if ((sq_cnt & 1) || (dq_cnt & 1))
		return (-1);
	return (0);
}
