/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 20:55:11 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/11 21:01:26 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	moving_two_pointers(char **start, char **end)
{
	while (**end && is_space(**end))
		++(*end);
	*start = *end;
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
				new = tokenize_redirect(&start, &end, &token, e);
			else if (ft_strchr("|", *start))
				new = tokenize_pipe(&end, &token);
			else if (ft_strchr("*", *start))
				new = tokenize_star(&end, &token);
			else // word, quote, $
				new = tokenize_word(&start, &end, &token, e);
			if (new == NULL)
				return (WRONG_ACTION);
			p->next = new;
			p = p->next;
		}
	}
	return (OK);
}
