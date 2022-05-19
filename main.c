/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:16:18 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/19 16:46:40 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

void	handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		if (rl_on_new_line() == -1)
			exit(1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handler_not_redis(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		if (rl_on_new_line() == -1)
			exit(1);
		rl_replace_line("", 0);
	}
}

void	init_token_header(t_token **token_header)
{
	*token_header = malloc(sizeof(t_token));
	if (*token_header == NULL)
		exit(1);
	(*token_header)->s = NULL;
	(*token_header)->file_name = NULL;
	(*token_header)->next = NULL;
}

void	init_tree(t_ast **tree)
{
	*tree = malloc(sizeof(t_ast));
	if (tree == NULL)
		exit(1);
	(*tree)->token = NULL;
	(*tree)->left = NULL;
	(*tree)->right = NULL;
	(*tree)->tree_type = TREE_PIPE;
	(*tree)->pipe_cnt = -1;
	(*tree)->root = *tree;
	(*tree)->std_fd[0] = dup(STDIN_FILENO);
	(*tree)->std_fd[1] = dup(STDOUT_FILENO);
}

static void wait_child(void)
{
	int	status;

	while (waitpid(-1, &status, 0) >= 0)
		;
	g_exit_code = status >> 8;
}

void	eof_exit(int col, int row)
{
	move_cursor(col, row);
	printf("exit\n");
	close(STDOUT_FILENO);
}

int main(int argc, char **argv, char **envp)
{
	char			*line;
	t_token			*token_header; // token 리스트의 헤더.
	t_ast			*tree;
	t_envs			e;
	char			*s;
	int				row;
	int				col;

	row = 0;
	col = 0;
	if (argc > 1)
		return (1);
	(void)argv;
	(void)envp;
	input_env(&e, envp);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		turn_off_echoctl();
		disable_canonical();
		get_position(&col, &row);
		enable_canonical();
		line = readline("minishell$ ");
		if (!line)
		{
			free_envs(&e);
			eof_exit(col, row);
			return (0);
		}
		init_token_header(&token_header);
		s = tokenizing(line, token_header, &e);
		if (s != PASS)
		{
			printf("%s\n", s);
			free_token(token_header);
		}
		else
		{
			// print_token_list(token_header);
			init_tree(&tree);
			s = parsing(tree, token_header);
			if (s != PASS)
			{
				printf("%s\n", s);
				free_token(token_header);
				free_tree(tree);
			}
			else
			{
				signal(SIGINT, handler_not_redis);
				tree_searching(tree, &e);
				dup_check(tree->root->std_fd[0], STDIN_FILENO);
				dup_check(tree->root->std_fd[1], STDOUT_FILENO);
				close(tree->root->std_fd[0]);
				close(tree->root->std_fd[1]);
				free_token(token_header);
				free_tree(tree);
			}
		}
		add_history(line);
		free(line);
		line = NULL;
		wait_child();
		signal(SIGINT, handler);
	}
	return (0);
}
