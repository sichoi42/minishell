#include "../minishell.h"
#include <stdlib.h>

// parsing tree를 만들 때 token 메모리를 복제하는 함수.

t_token	*ft_token_dup(t_token *src)
{
	t_token	*dst;

	dst = malloc(sizeof(t_token));
	if (dst == NULL)
		return (NULL);
	dst->token = src->token;
	dst->type = src->type;
	dst->s = ft_strndup(src->s, ft_strlen(src->s));
	dst->next = NULL;
	return (dst);
}

// TREE_PIPE, TREE_BUNDLE, TREE_RE, TREE_CMD
char	*get_tree_type_str(enum e_tree_type tree_type)
{
	char	*s;

	if (tree_type == TREE_PIPE)
		s = "TREE_PIPE";
	else if (tree_type == TREE_BUNDLE)
		s = "TREE_BUNDLE";
	else if (tree_type == TREE_RE)
		s = "TREE_RE";
	else if (tree_type == TREE_CMD)
		s = "TREE_CMD";
	return (s);
}
