/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 20:55:11 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/12 18:02:26 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

// space를 건너뛰고 다음 토큰이 있는 위치로 two pointers를 이동.
void	moving_two_pointers(char **start, char **end)
{
	while (**end && is_space(**end))
		++(*end);
	*start = *end;
}

// redirect, pipe, star, word의 시작 부분을 만날 때 마다 해당 토큰을 하나씩 처리하고 반환함.
t_token	*token_split(char **start, char **end, enum e_token *token, t_envs *e)
{
	t_token	*new;

	if (ft_strchr("><", **start))
		new = tokenize_redirect(start, end, token, e);
	else if (ft_strchr("|", **start))
		new = tokenize_pipe(end, token);
	else if (ft_strchr("*", **start))
		new = tokenize_star(end, token);
	else
		new = tokenize_word(start, end, token, e);
	return (new);
}

// 받아온 line에서 토큰을 하나씩 받아서 linked list에 연결.
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
			new = token_split(&start, &end, &token, e);
			if (new == NULL)
				return (WRONG_ACTION);
			p->next = new;
			p = p->next;
		}
	}
	return (OK);
}
