/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 18:59:08 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/17 14:15:56 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

void	parsing(t_ast *tree, t_token *token_header)
{
	t_token	*p;

	p = token_header->next;
	while (p)
	{
		syntax_pipe(tree, p, tree->root);
		p = p->next;
	}
}

void	execute_something(t_ast *node, t_envs *e)
{
	t_token	*t;

	(void)e;
	printf("%s\n", get_tree_type_str(node->tree_type));
	if (node->token != NULL)
	{
		t = node->token;
		if (node->tree_type == TREE_CMD)
		{
			// 커맨드일 때, next가 있기 때문에
			printf("command in: ");
			while (t)
			{
				printf("%s ", t->s);
				t = t->next;
			}
			printf("\n");
			printf("argc: %d\n", node->argc);
			printf("pipe: %d\n", node->root->pipe_cnt);
		}
		else
		{
			// 커맨드 그 외의 것들(pipe, redirection). next == NULL
			printf("not command: ");
			printf("%s\n", t->s);
		}
		print_token_list(t);
	}
	printf("------------------\n");
}

// preorder로 트리를 탐색.
void	tree_searching(t_ast *node, t_envs *e)
{
	execute_something(node, e);
	if (node->left != NULL)
		tree_searching(node->left, e);
	if (node->right != NULL)
		tree_searching(node->right, e);
}
