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
// token: WORD
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
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	init_term(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
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
	(*tree)->pipe_cnt = 0;
	(*tree)->root = *tree;
}

int main(int argc, char **argv, char **envp)
{
	char			*line;
	t_token			*token_header; // token 리스트의 헤더.
	t_ast			*tree;

	if (argc > 1)
		return (1);
	(void)argv;
	(void)envp;
	init_term();
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		line = readline("minishell> ");
		if (line)
		{
			init_token_header(&token_header);
			if (tokenizing(line, token_header) == WRONG_ACTION)
			{
				printf("detected unclosed quote\n");
				free_token(token_header);
			}
			else
			{
				print_token_list(token_header);
				init_tree(&tree);
				parsing(tree, token_header);
				free_token(token_header);
				free_tree(tree);
			}
			add_history(line);
			free(line);
			line = NULL;
		}
		else
		{
			printf("\033[1A");
			printf("\033[11C");
			printf("exit\n");
			return (1);
		}
	}
	return (0);
}
