#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	is_redirection(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int is_star(char c)
{
	if (c == '*')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

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

t_token	*split_word(char **start, char **end, enum e_token *token)
{
	t_token	*new;
	int		offset;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		exit(1);
	new->next = NULL;
	while (**end && !is_space(**end) && !is_pipe(**end) && !is_redirection(**end))
		++(*end);
	offset = *end - *start;
	new->s = ft_strndup(*start, offset);
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
	while (**end && is_space(**end))
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
			if (is_redirection(*start))
				new = split_redirect(&start, &end, &token);
			else if (is_pipe(*start))
				new = split_pipe(&end, &token);
			else if (is_star(*start))
				new = split_star(&end, &token);
			else if (is_quote(*start))
				new = split_quote(&start, &end, &token);
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
