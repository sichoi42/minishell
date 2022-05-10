#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

char	*get_env(char **start, char *end, t_envs *e)
{
	char	*key;
	char	*value;

	key = ft_strndup(*start, end - *start);
	printf("%s\n", key);
	value = ft_strdup(get_env_value(e, key));
	free(key);
	return (value);
}

char	*split_word_in_dollar(char **start, char **end, enum e_token *token, t_envs *e)
{
	char	*s;
	char	*value;

	++(*end);
	if (**end == 0 || ft_strchr(" \"\'|><\\$", **end))
	{
		s = ft_strdup("$");
	}
	else if (**end == '?')
	{
		s = ft_strndup("$?", 2);
		*token |= T_EXIT_CODE;
		++(*end);
	}
	else
	{
		while (**end && !ft_strchr_ig_blsh(" \"\'|><\\$", *end, end))
			++(*end);
		++(*start);
		value = get_env(start, *end, e);
		s = ft_strndup(value, ft_strlen(value));
		free(value);
	}
	return (s);
}

char	*split_word_in_quote(char **start, char **end, enum e_token *token, t_envs *e)
{
	char	*s;
	char	*d_end;
	char	*value;
	char	quote;

	s = NULL;
	quote = **start;
	++(*start);
	while (**start != **end || (**start == quote && *(*start - 1) == '\\'))
	{
		if (**start == '$' && quote == '\"')
		{
			d_end = *start + 1;
			if (*d_end == '?')
			{
				*token |= T_EXIT_CODE;
				++d_end;
				s = ft_strnjoin(s, "$?", 2);
			}
			else
			{
				while (*d_end && !ft_strchr_ig_blsh(" \"\'|><\\$", d_end, &d_end))
					++d_end;
				value = get_env(start, d_end, e);
				s = ft_strnjoin(s, value, ft_strlen(value));
				free(value);
			}
			*start = d_end - 1;
		}
		else
			s = ft_strnjoin(s, *start, 1);
		++(*start);
	}
	if (s == NULL)
		s = ft_strdup("");
	++(*end);
	return (s);
}

void	move_quote_end(char **start, char **end)
{
	while (**end)
	{
		if (**end == **start)
		{
			if (*(*end - 1) != '\\')
				break ;
		}
		++(*end);
	}
}

t_token	*split_word(char **start, char **end, enum e_token *token, t_envs *e)
{
	t_token	*new;
	char	*s;
	char	*q_s;
	char	*d_s;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		exit(1);
	new->next = NULL;
	s = NULL;
	while (**end && !ft_strchr(" |><", **end))
	{
		if (**end && !ft_strchr("\"\'$", **end)) // 일반문자인 경우
		{
			while (**end && !ft_strchr_ig_blsh(" \"\'$|><", *end, end)) // 일반문자 사이 offset 구함.
				++(*end);
			s = ft_strnjoin(s, *start, *end - *start); // 일반문자열 join.
			*start = *end;
		}
		if (**end && ft_strchr("\"\'", **end)) // quote인 경우
		{
			++(*end);
			move_quote_end(start, end); // quote 사이 offset 구함.
			if (**end == 0) // unclosed quote일 경우 예외 처리.
			{
				free(s);
				free(new);
				return (NULL);
			}
			else
			{
				q_s = split_word_in_quote(start, end, token, e); // 환경변수 정보 포함 quote안의 내용 받아옴.
				s = ft_strnjoin(s, q_s, ft_strlen(q_s)); // quot문자열 join
				free(q_s);
			}
			*start = *end;
		}
		if (**end && **end == '$') // $인 경우
		{
			d_s = split_word_in_dollar(start, end, token, e); // 환경변수 정보를 받아옴.
			s = ft_strnjoin(s, d_s, ft_strlen(d_s)); // 해당 값을 join.
			free(d_s);
			*start = *end;
		}
	}
	if (s == NULL)
		return (NULL);
	new->s = s;
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

t_token	*split_redirect(char **start, char **end, enum e_token *token, t_envs *e)
{
	t_token	*new;

	get_redirect_token(start, end, token);
	moving_two_pointers(start, end);
	if (ft_strchr("|", **start))
		new = split_pipe(end, token);
	else if (ft_strchr("*", **start))
		new = split_star(end, token);
	else
		new = split_word(start, end, token, e);
	new->type |= REDIRECT;
	new->type &= ~ARGS;
	return (new);
}

int	tokenizing(char *line, t_token *t, t_envs *e)
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
			if (ft_strchr("><", *start))
				new = split_redirect(&start, &end, &token, e);
			else if (ft_strchr("|", *start))
				new = split_pipe(&end, &token);
			else if (ft_strchr("*", *start))
				new = split_star(&end, &token);
			else // word, quote, $
				new = split_word(&start, &end, &token, e);
			if (new == NULL)
				return (WRONG_ACTION);
			p->next = new;
			p = p->next;
		}
	}
	return (OK);
}
