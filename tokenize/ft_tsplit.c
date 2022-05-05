#include "../minishell.h"
#include <stdlib.h>

// WORD 토큰화.
// line중 word에 해당하는 토큰 생성.
t_token	*split_word(char *s, int *i)
{
	int			j;
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->token = T_WORD;
	new->type = ARGS;
	j = *i;
	while (*(s + *i) && *(s + *i) != ' ')
		++(*i);
	new->s = ft_strndup(s + j, *i - j);
	if (new->s == NULL)
		return (NULL);
	return (new);
}

// QUOTE, DQUOTE 토큰화.
// single인지 double quote인지에 따라 해당 토큰을 반환.
enum Token	get_quote_token(char *s, int *i, int *sq_cnt, int *dq_cnt)
{
	enum Token token;

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

// line중 single quote와 double quote로 감싸진 token 생성.
t_token	*split_quote(char *s, int *i, int *sq_cnt, int *dq_cnt)
{
	int			j;
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->token = get_quote_token(s, i, sq_cnt, dq_cnt);
	new->type = ARGS;
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

// line중 $로 시작하는 token 생성.
t_token	*split_dollar(char *s, int *i)
{
	int		j;
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->token = T_DOLLAR;
	new->type = ARGS;
	++(*i);
	j = *i;
	while (*(s + *i) && *(s + *i) != ' ')
		++(*i);
	new->s = ft_strndup(s + j, *i - j);
	if (new->s == NULL)
		return (NULL);
	return (new);
}

// PIPE, STAR 토큰화.
// line중 한글자로 이루어진 token(|, *)을 생성.
t_token	*split_one_elem(char *s, int *i)
{
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	if (s[*i] == T_PIPE)
	{
		new->token = T_PIPE;
		new->type = PIPE;
	}
	else
	{
		new->token = T_STAR;
		new->type = STAR;
	}
	new->s = ft_strndup(&s[*i], 1);
	if(new->s == NULL)
		return (NULL);
	++(*i);
	return (new);
}

// 리다이렉션들 토큰화.
// 어떤 리다이렉션인지에 따라 해당 토큰을 반환.
enum Token	get_redirect_token(char *s, int *i)
{
	enum Token token;

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

// line중 리다이렉션에 해당하는 토큰 생성.
t_token	*split_redirect(char *s, int *i)
{
	int		j;
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->token = get_redirect_token(s, i);
	new->type = REDIRECT;
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
		else if (*(s + i) == T_DOLLAR)
			new = split_dollar(s, &i);
		else if (*(s + i) == T_PIPE || *(s + i) == T_STAR)
			new = split_one_elem(s, &i); // |, *
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
