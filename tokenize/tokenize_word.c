/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 21:01:10 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/11 22:10:29 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	join_normal_word(char **s, char **start, char **end)
{
	if (**end && !ft_strchr("\"\'$", **end)) // 현재 문자가 일반문자인 경우
	{
		while (**end && !is_space(**end) && !ft_strchr("\"\'$|><", **end)) // 일반문자 사이 offset 구함.
			++(*end);
		*s = ft_strnjoin(*s, *start, *end - *start); // 일반문자열 join.
		*start = *end;
	}
}

void	move_quote_end(char **start, char **end)
{
	++(*end);
	while (**end)
	{
		if (**end == **start)
			break ;
		++(*end);
	}
}

t_token	*treat_unclosed_quote(t_token **new, char **s)
{
	free(*s);
	free(*new);
	return (NULL);
}

void	fill_word_info(t_token **new, char *s, enum e_token *token)
{
	if (s == NULL)
		s = ft_strdup("");
	(*new)->s = s;
	*token |= T_WORD;
	(*new)->type = ARGS;
	(*new)->token = *token;
}

t_token	*tokenize_word(char **start, char **end, enum e_token *token, t_envs *e)
{
	t_token	*new;
	char	*s;

	new = create_new_token();
	s = NULL;
	while (**end && !is_space(**end) && !ft_strchr("|><", **end)) // space, |, ><을 발견할 때 까지 loop.
	{
		join_normal_word(&s, start, end); // 현재 문자가 일반문자인 경우
		if (**end && ft_strchr("\"\'", **end)) // quote인 경우
		{
			move_quote_end(start, end); // quote 사이 offset 구함.
			if (**end == 0) // unclosed quote일 경우 예외 처리.
				return (treat_unclosed_quote(&new, &s));
			else
				s = ft_strjoin(s, tokenize_word_in_quote(start, end, token, e));// 환경변수 정보 포함 quote안의 내용을 받아와 s에 연결.
		}
		if (**end && **end == '$') // $인 경우
			s = ft_strjoin(s, tokenize_word_in_dollar(start, end, token, e)); // 환경변수 정보를 받아와 s에 연결.
	}
	fill_word_info(&new, s, token);
	if (**end == 0)
		*start = *end;
	else
		*start = *end + 1;
	return (new);
}
