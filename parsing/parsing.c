/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 18:59:08 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/17 15:20:22 by swi              ###   ########.fr       */
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
	int i;

	o.opers = malloc_array(sizeof(char *), node->root->argc + 1);
	o.opers[node->root->argc] = NULL;
	i = -1;
	while (node->token)
	{
		o.opers[++i] = malloc_array(sizeof(char), ft_strlen(node->token->s) + 1);
		ft_strcpy(node->token->s, o.opers[i]);
		node->token = node->token->next;
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

	printf("tree_type: %s\n", get_tree_type_str(node->tree_type));
	if (node->tree_type == TREE_BUNDLE)
	{
		init_pipe(node->pipe_fd);
		dup_check(node->std_fd[1], STDOUT_FILENO);
		if (node->root->pipe_cnt > 0)
			make_pipe(node->pipe_fd);
	}
	if (node->token != NULL)
	{
		t = node->token;
		if (node->tree_type == TREE_CMD)
		{
			exe_command(node, e);
			printf("command in: ");
			while (t)
			{
				printf("%s ", t->s);
				t = t->next;
			}
			printf("\n");
			printf("argc: %d\n", node->root->argc);
			printf("pipe: %d\n", node->root->pipe_cnt);
			printf("pwd: %s\n", e->pwd);
		}
		else
		{
			if (t->type == REDIRECT)
				red_open_file(t->token, t->s);
			printf("not command: ");
			printf("%s\n", t->s);
		}
		print_token_list(t);
	}
	printf("------------------\n");
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
