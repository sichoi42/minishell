/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word_in_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 20:59:12 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/11 22:23:13 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stddef.h>

char	*tokenize_word_in_quote(char **start, char **end, enum e_token *token, t_envs *e)
{
	char	*s;
	char	*value;
	char	quote;

	s = NULL;
	quote = **start;
	++(*start);
	// start가 end문자가 될 때까지 인덱스를 넘기되, 중간에 bkslsh quote를 만나면 무시.
	while (**start != **end)
	{
		if (**start == '$' && quote == '\"') // double quote안에서 $를 만났을 때,
		{
			value = tokenize_word_in_quote_in_dollar(start, end, token, e);
			s = ft_strjoin(s, value);
		}
		else // 그 외는 한글짜씩 join해 옴.
			s = ft_strnjoin(s, *start, 1);
		++(*start);
	}
	if (s == NULL)
		s = ft_strdup("");
	++(*end);
	*start = *end;
	return (s);
}
