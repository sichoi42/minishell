#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

char	*find_quote(char *s, char c)
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

t_token	*split_quote(char **start, char **end, enum e_token *token)
{
	t_token	*new;
	int		offset;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		exit(1);
	new->next = NULL;
	if (**start == '\'')
		*token |= T_QUOTE;
	else
		*token |= T_DQUOTE;
	*end = find_quote(*start + 1, **start);
	if (*end == NULL)
	{
		free(new);
		return (NULL);
	}
	offset = *end - *start - 1;
	new->s = ft_strndup(*start + 1, offset);
	if (new->s == NULL)
		exit(1);
	++(*end);
	new->token = *token;
	new->type = ARGS;
	if (**end == 0)
		*start = *end;
	else
		*start = *end + 1;
	return (new);
}

int	get_env_len()
{
	return (10);
}

t_token	*split_word(char **start, char **end, enum e_token *token)
{
	t_token	*new;
	int		offset;
	char	*q_start;
	char	*q_end;
	char	*d_start;
	char	*d_end;
	char	*key;
	char	*value;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		exit(1);
	new->next = NULL;
	while (**end && !ft_strchr(" |><", **end))
		++(*end);
	q_start = *start;
	while (q_start && !ft_strchr(" \"\'|><", q_start))
		++q_start;
	if (q_start)
	{
		q_end = q_start + 1;
		while (q_end && !ft_strchr(" \"\'|><", q_end))
			++q_end;
		if (q_end == NULL)
			return (NULL);
		d_start = q_start + 1;
		while (d_start && !ft_strchr(" $|><", d_start))
			++d_start;
		d_end = d_start;
		while (d_end && !ft_strchr(" |><", d_end))
			++d_end;
		key = ft_strndup(d_start, d_end - d_start);
		value = ft_strndup(value, ft_strlen(value));
	}
	else
		;


	// offset = *end - *start;
	// new->s = ft_strndup(*start, offset);
	*token |= T_WORD;
	new->type = ARGS;
	new->token = *token;
	if (**end == 0)
		*start = *end;
	else
		*start = *end + 1;
	return (new);
}

void	get_redirect_token(char **start, char **end, enum e_token *token)
{
	if (*(*end + 1) && *(*end + 1) == **end)
	{
		if (**end == '>')
			*token |= T_RE_APPEND_OUTPUT;
		else
			*token |= T_RE_HEREDOC;
		++(*end);
	}
	else
	{
		if (**end == '>')
			*token |= T_RE_OUTPUT;
		else
			*token |= T_RE_INPUT;
	}
	++(*end);
	*start = *end;
}

t_token	*split_pipe(char **end, enum e_token *token)
{
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		exit(0);
	new->next = NULL;
	new->s = ft_strndup(*end, 1);
	if (new->s == NULL)
		exit(0);
	*token |= T_PIPE;
	new->type = PIPE;
	new->token = *token;
	++(*end);
	return (new);
}

t_token	*split_star(char **end, enum e_token *token)
{
	t_token		*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		exit(0);
	new->next = NULL;
	new->s = ft_strndup(*end, 1);
	if (new->s == NULL)
		exit(0);
	*token |= T_STAR;
	new->type = STAR;
	new->token = *token;
	++(*end);
	return (new);
}

void	moving_two_pointers(char **start, char **end)
{
	while (**end && ft_strchr(" ", **end))
		++(*end);
	*start = *end;
}

t_token	*split_redirect(char **start, char **end, enum e_token *token)
{
	t_token	*new;

	get_redirect_token(start, end, token);
	moving_two_pointers(start, end);
	if (is_quote(**start))
		new = split_quote(start, end, token);
	else if (is_pipe(**start))
		new = split_pipe(end, token);
	else if (is_star(**start))
		new = split_star(end, token);
	else
		new = split_word(start, end, token);
	new->type |= REDIRECT;
	new->type &= ~ARGS;
	return (new);
}

int	tokenizing(char *line, t_token *t)
{
	char			*start;
	char			*end;
	enum e_token	token;
	t_token			*new;
	t_token			*p;

	end = line;
	start = end;
	p = t;
	while (*end)
	{
		token = T_INIT;
		moving_two_pointers(&start, &end);
		if (*start)
		{
			if (ft_strchr(">", *start))
				new = split_redirect(&start, &end, &token);
			else if (ft_strchr("|", *start))
				new = split_pipe(&end, &token);
			else if (ft_strchr("*", *start))
				new = split_star(&end, &token);
			else
				new = split_word(&start, &end, &token);
			if (new == NULL)
				return (WRONG_ACTION);
			p->next = new;
			p = p->next;
		}
	}
	return (OK);
}
