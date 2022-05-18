/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:16:18 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/18 18:25:39 by sichoi           ###   ########.fr       */
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
#include <termios.h>
#include <errno.h>

// 매개변수로 명령어를 입력하여 토큰화된 결과를 출력.
// ex
// >> "ls -al > a | grep abc"  (readline 함수에서 명령어를 한 문자열을 받는 것을 고려하여 전체를 한 문자열로 묶어서 전달해야함.)
// ls
// token: T_WORD
// type: ARGS
// ----------
// -al
// token: T_WORD
// type: ARGS
// ----------
// a
// token: T_RE_OUTPUT
// type: REDIRECT
// ----------
// |
// token: T_PIPE
// type: PIPE
// ----------
// grep
// token: WORD2
// type: ARGS
// ----------
// abc
// token: WORD
// type: ARGS
// ----------

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

void	turn_off_echoctl(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO))
	{
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	else if (isatty(STDOUT_FILENO))
	{
		tcgetattr(STDOUT_FILENO, &term);
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDOUT_FILENO, TCSANOW, &term);
	}
	else if (isatty(STDERR_FILENO))
	{
		tcgetattr(STDERR_FILENO, &term);
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDERR_FILENO, TCSANOW, &term);
	}
}

void	turn_on_echoctl(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	init_token_header(t_token **token_header)
{
	*token_header = malloc(sizeof(t_token));
	if (*token_header == NULL)
		exit(1);
	(*token_header)->s = NULL;
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

int main(int argc, char **argv, char **envp)
{
	char			*line;
	t_token			*token_header; // token 리스트의 헤더.
	t_ast			*tree;
	t_envs			e;
	char			*s;

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
		// printf("\033[6n");
		// TODO: 이 시점의 커서 위치를 파일에다가 저장 후,
		// eof를 만났을 때 그 시점의 커서 위치의 col값 + 11만큼으로 커서 이동.
		line = readline("minishell$ ");
		if (line)
		{
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
			write(STDOUT_FILENO, "\033[K", ft_strlen("\033[K"));
		}
		else
		{
			free_envs(&e);
			printf("\033[1A");
			printf("\033[11C");
			printf("exit\n");
			return (1);
		}
		wait_child();
		signal(SIGINT, handler);
	}
	return (0);
}
