#include "../minishell.h"
#include <stddef.h>
#include <stdio.h>

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
int main(int argc, char **argv)
{
	char	*s;
	t_token	*p;
	t_token	token_header; // token 리스트의 헤더.
	t_ast	tree;

	if (argc != 2)
		return (1);
	token_header.next = NULL;
	token_header.s = NULL;
	s = argv[1];
	if (ft_tsplit(&token_header, s) == -1)
		return (1);
	p = token_header.next;
	while (p)
	{
		printf("%s\n", p->s);
		printf("token: %s\n", get_token_str(p->token));
		printf("type: %s\n", get_type_str(p->type));
		printf("-----------------\n");
		p = p->next;
	}
	tree.token = NULL;
	tree.left = NULL;
	tree.right = NULL;
	tree.tree_type = TREE_PIPE;
	tree.pipe_cnt = 0;
	tree.root = &tree;
	parsing(&tree, &token_header);
	return (0);
}
