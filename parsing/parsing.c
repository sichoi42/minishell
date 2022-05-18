/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 18:59:08 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/17 23:09:32 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

void exe_command(t_ast *node, t_envs *e)
{
	t_oper o;
	t_paths p;
	t_token	*t;
	int i;

	o.opers = malloc_array(sizeof(char *), node->argc + 1);
	o.opers[node->argc] = NULL;
	i = -1;
	t = node->token;
	while (t)
	{
		o.opers[++i] = malloc_array(sizeof(char), ft_strlen(t->s) + 1);
		ft_strcpy(t->s, o.opers[i]);
		t = t->next;
	}
	// builtin 후에 path를 찾는 방식으로 로직수정 필요.
	find_path(&(p.paths), &(p.max_len), e);
	o.oper_path = make_oper(o.opers, p.max_len, p.paths);
	if (node->root->pipe_cnt <= -1 && built_in_check(&o, e) == -1)
		exe_oper(&o, node, e);
	else if (node->root->pipe_cnt >= 0)
		exe_oper(&o, node, e);
	free(o.oper_path);
	free_double_char(o.opers);
	free_double_char(p.paths);
}

void execute_something(t_ast *node, t_envs *e)
{
	t_token *t;

	if (node->tree_type == TREE_BUNDLE)
	{
		init_pipe(node->root->pipe_fd);
		dup_check(node->root->std_fd[1], STDOUT_FILENO);
		if (node->root->pipe_cnt > 0)
			make_pipe(node->root->pipe_fd);
	}
	if (node->token != NULL)
	{
		t = node->token;
		if (node->tree_type == TREE_CMD)
		{
			turn_on_echoctl();
			exe_command(node, e);
		}
		else
		{
			if (t->type == REDIRECT)
				red_open_file(t->token, t->s);
		}
	}
}

/*
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
*/

// preorder로 트리를 탐색.
void	tree_searching(t_ast *node, t_envs *e)
{
	execute_something(node, e);
	if (node->left != NULL)
		tree_searching(node->left, e);
	if (node->right != NULL)
		tree_searching(node->right, e);
}
