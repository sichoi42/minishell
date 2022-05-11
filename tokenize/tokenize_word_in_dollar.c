/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word_in_dollar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 20:57:57 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/11 21:56:59 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

char	*key_to_value(char **start, char *end, t_envs *e)
{
	char	*key;
	char	*value;

	key = ft_strndup(*start, end - *start);
	value = ft_strdup(get_env_value(e, key));
	free(key);
	return (value);
}

char	*tokenize_word_in_dollar(char **start, char **end, enum e_token *token, t_envs *e)
{
	char	*s;

	++(*end);
	if (**end == 0 || is_space(**end) || ft_strchr("\"\'|><$", **end)) // $다음이 null이거나 특수문자인 경우,
	{
		if (ft_strchr("\"\'", **end)) // $다음이 quote일 경우, 빈 문자열 할당.
			s = ft_strdup("");
		else
			s = ft_strdup("$"); // 그 외는 $만 할당
	}
	else if (**end == '?') // $ 다음이 ?일 때,
	{
		s = ft_strndup("$?", 2); // $?를 할당하고 T_EXIT_CODE 토큰 추가.
		*token |= T_EXIT_CODE;
		++(*end);
	}
	else // $다음이 일반문자인 경우, key값을 알아내기 위해 문자의 끝 위치 찾아감.
	{
		while (**end && !is_space(**end) && !ft_strchr("\"\'|><$", **end))
			++(*end);
		++(*start);
		s = key_to_value(start, *end, e); // key로 value값을 받아옴.
	}
	*start = *end;
	return (s);
}

char	*tokenize_word_in_quote_in_dollar(char **start, char **end, enum e_token *token, t_envs *e)
{
	char	*d_end;
	char	*s;

	d_end = *start + 1;
	if (*d_end == '?') // $다음이 ?인 경우 T_EXIT_CODE 토큰을 추가하고 $? 문자열 반환.
	{
		*token |= T_EXIT_CODE;
		++d_end;
		s = ft_strdup("$?");
	}
	else if (is_space(*d_end) || ft_strchr("\"\'|><$", *d_end)) // $다음이 특수 문자인 경우
	{
		if (ft_strchr("\"\'", **end)) // $다음이 quote인 경우, 빈 문자열 추가. 그 외는 $그대로 처리.
			s = ft_strdup("");
		s = ft_strdup("$");
	}
	else // $다음이 일반 문자인 경우, key값을 알아내기 위해 문자의 끝 위치 찾아감.
	{
		while (*d_end && !is_space(*d_end) && !ft_strchr("\"\'|><$", *d_end))
			++d_end;
		++(*start);
		s = key_to_value(start, d_end, e);
	}
	*start = d_end - 1;
	return (s);
}
