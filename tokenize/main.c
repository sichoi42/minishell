#include "utils.h"
#include <stddef.h>
#include <stdio.h>

// 매개변수로 명령어를 입력하여 토큰화된 결과를 출력.
// ex
// >> "ls -al > a | grep abc"  (readline 함수에서 명령어를 한 문자열을 받는 것을 고려하여 전체를 한 문자열로 묶어서 전달해야함.)
// ls
// WORD
// ----------
// -al
// WORD
// ----------
// a
// RE_OUTPUT
// ----------
// |
// PIPE
// ----------
// grep
// WORD
// ----------
// abc
// WORD
// ----------
int main(int argc, char **argv)
{
	char *s = argv[1];
	argc = argc;
	t_token	token_header; // token 리스트의 헤더.

	token_header.prev = NULL;
	token_header.next = NULL;
	token_header.s = NULL;
	if (ft_tsplit(&token_header, s) == -1)
		return (1);
	t_token	*p;
	p = token_header.next;
	while (p)
	{
		printf("%s\n", p->s);
		printf("%s\n", get_token_str(p->token));
		printf("-----------------\n");
		p = p->next;
	}
}
