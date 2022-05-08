#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

int	syntax_pipe(t_ast *node, t_token *t, t_ast *root)
{
	int	i;

	i = -1;
	while (++i < root->pipe_cnt)
		node = node->right;
	if (t->type & PIPE)
	{
		++(node->root->pipe_cnt);
		node->right = malloc(sizeof(t_ast));
		if (node->right == NULL)
			return (-1);
		node->right->left = NULL;
		node->right->right = NULL;
		node->right->root = root;
		node->right->tree_type = TREE_PIPE;
		node->right->token = ft_token_dup(t);
	}
	else
	{
		if (node->left == NULL) // bundle에 최초 방문 시,
		{
			node->left = malloc(sizeof(t_ast));
			if (node->left == NULL)
				return (-1);
			node->left->left = NULL;
			node->left->right = NULL;
			node->left->root = root;
			node->left->token = NULL;
			node->left->tree_type = TREE_BUNDLE;
		}
		if (syntax_bundle(node->left, t, root) == -1)
			return (-1);
	}
	return (0);
}

int	syntax_bundle(t_ast *node, t_token *t, t_ast *root)
{
	if (t->type & ARGS || t->type & STAR)
	{
		if (node->right == NULL) // cmd로 최초 방문 시,
		{
			node->right = malloc(sizeof(t_ast));
			if (node->right == NULL)
				return (-1);
			node->right->left = NULL;
			node->right->right = NULL;
			node->right->root = root;
			node->right->token = NULL;
			node->right->tree_type = TREE_CMD;
		}
		syntax_cmd(node->right, t, root);
	}
	else if (t->type & REDIRECT)
	{
		if (node->left == NULL) // redirect로 최초 방문 시,
		{
			node->left = malloc(sizeof(t_ast));
			if (node->left == NULL)
				return (-1);
			node->left->left = NULL;
			node->left->right = NULL;
			node->left->root = root;
			node->left->token = NULL;
			node->left->tree_type = TREE_RE;
		}
		syntax_redirect(node->left, t, root);
	}
	return (0);
}

int	syntax_redirect(t_ast *node, t_token *t, t_ast *root)
{
	if (node->left == NULL)
	{
		node->left = malloc(sizeof(t_ast));
		if (node->left == NULL)
			return (-1);
		node->left->left = NULL;
		node->left->right = NULL;
		node->left->root = root;
		node->left->token = NULL;
		node->left->tree_type = TREE_RE;
		syntax_decision_redirect(node->left, t, root);
	}
	else
	{
		if (t->type & REDIRECT)
		{
			if (node->right == NULL)
			{
				node->right = malloc(sizeof(t_ast));
				if (node->right == NULL)
					return (-1);
				node->right->left = NULL;
				node->right->right = NULL;
				node->right->root = root;
				node->right->token = NULL;
				node->right->tree_type = TREE_RE;
			}
			if (syntax_redirect(node->right, t, root) == -1)
				return (-1);
		}
	}
	return (0);
}

void	syntax_decision_redirect(t_ast *node, t_token *t, t_ast *root)
{
	node->token = ft_token_dup(t);
	if (t->token & T_RE_OUTPUT)
		;
	else if (t->token & T_RE_INPUT)
		;
	else if (t->token & T_RE_APPEND_OUTPUT)
		;
	else if (t->token & T_RE_HEREDOC)
		;
}

int	syntax_cmd(t_ast *node, t_token *t, t_ast *root)
{
	t_token	*p;

	if (node->token == NULL)
		node->token = ft_token_dup(t); // 메모리를 복사.
	else
	{
		p = node->token;
		while (p->next)
			p = p->next;
		p->next = ft_token_dup(t);
	}
	return (0);
}

void	parsing(t_ast *tree, t_token *token_header)
{
	t_token	*p;

	p = token_header->next;
	while (p)
	{
		syntax_pipe(tree, p, tree->root);
		p = p->next;
	}
	tree_searching(tree);
}

void	execute_something(t_ast *node)
{
	t_token	*t;

	printf("tree_type: %s\n", get_tree_type_str(node->tree_type));
	if (node->token != NULL)
	{
		t = node->token;
		if (node->tree_type == TREE_CMD)
		{
			while (t)
			{
				printf("%s ", t->s);
				t = t->next;
			}
			printf("\n");
		}
		else
			printf("%s\n", t->s);
		print_token_list(t);
	}
	printf("------------------\n");
}

void	tree_searching(t_ast *node)
{
	execute_something(node);
	if (node->left != NULL)
		tree_searching(node->left);
	if (node->right != NULL)
		tree_searching(node->right);
}

void	free_tree(t_ast	*node)
{
	if (node)
	{
		free_tree(node->left);
		free_tree(node->right);
		free_token(node->token);
		free(node);
		node = NULL;
	}
}
