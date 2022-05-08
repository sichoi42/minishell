#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

void	print_token_str(enum e_token token)
{
	if (token & T_WORD)
		printf("%s, ", "T_WORD");
	if (token & T_QUOTE)
		printf("%s, ", "T_QUOTE");
	if (token & T_DQUOTE)
		printf("%s, ", "T_DQUOTE");
	if (token & T_PIPE)
		printf("%s, ", "T_PIPE");
	if (token & T_DOLLAR)
		printf("%s, ", "T_DOLLAR");
	if (token & T_STAR)
		printf("%s, ", "T_STAR");
	if (token & T_RE_INPUT)
		printf("%s, ", "T_RE_INPUT");
	if (token & T_RE_OUTPUT)
		printf("%s, ", "T_RE_OUTPUT");
	if (token & T_RE_HEREDOC)
		printf("%s, ", "T_RE_HEREDOC");
	if (token & T_RE_APPEND_OUTPUT)
		printf("%s, ", "T_RE_APPEND_OUTPUT");
	printf("\n");
}

void	print_type_str(enum e_type type)
{
	if (type & ARGS)
		printf("%s, ", "ARGS");
	if (type & PIPE)
		printf("%s, ", "PIPE");
	if (type & REDIRECT)
		printf("%s, ", "REDIRECT");
	if (type & STAR)
		printf("%s, ", "STAR");
	printf("\n");
}

void	print_token_list(t_token *t)
{
	t_token	*p;

	if (t)
	{
		p = t->next;
		while (p)
		{
			printf("%s\n", p->s);
			print_token_str(p->token);
			print_type_str(p->type);
			printf("-----------------\n");
			p = p->next;
		}
	}
}

void	free_token(t_token *t)
{
	t_token	*p;
	t_token	*tmp;

	if (t)
	{
		p = t;
		while (p)
		{
			if (p->next)
			{
				tmp = p->next;
				free(p->s);
				free(p);
				p = tmp;
			}
			else
			{
				free(p->s);
				free(p);
			}
			p = p->next;
		}
	}
}
