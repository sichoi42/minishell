
// 사용되는 token
enum e_token
{
	T_WORD, T_QUOTE = '\'', T_DQUOTE = '\"', T_DOLLAR = '$',
	T_RE_INPUT, // <
	T_RE_OUTPUT, // >
	T_RE_HEREDOC, // <<
	T_RE_APPEND_OUTPUT, // >>
	T_PIPE = '|',
	T_STAR = '*'
};

// 토큰의 type
enum e_type
{
	REDIRECT,
	PIPE,
	ARGS,
	STAR
};

// parse tree의 type
enum e_tree_type
{
	TREE_PIPE, TREE_BUNDLE, TREE_RE, TREE_CMD
};

// 각 토큰들을 linked list로 저장.
typedef struct	s_token
{
	char				*s;
	enum e_token		token;
	enum e_type			type;
	struct s_token		*next;
}	t_token;

typedef struct s_ast
{
	t_token				*token;
	struct s_ast		*left;
	struct s_ast		*right;
	struct s_ast		*root;
	enum e_tree_type	tree_type;
	int					pipe_cnt;
}	t_ast;

// in tokenize

// utils.c
int	ft_strlen(char *s);
char	*ft_strndup(char *src, int len);
char	*get_token_str(enum e_token token);
char	*get_type_str(enum e_type type);
void	print_token_list(t_token *t);

// ft_tsplit.c
t_token	*split_word(char *s, int *i);
enum e_token	get_quote_token(char *s, int *i, int *sq_cnt, int *dq_cnt);
t_token	*split_quote(char *s, int *i, int *sq_cnt, int *dq_cnt);
t_token	*split_dollar(char *s, int *i);
t_token	*split_one_elem(char *s, int *i);
enum e_token	get_redirect_token(char *s, int *i);
t_token	*split_redirect(char *s, int *i);
int	ft_tsplit(t_token *t, char *s);

// in parsing

// parsing.c
int	syntax_pipe(t_ast *node, t_token *t, t_ast *root);
int	syntax_bundle(t_ast *node, t_token *t);
int	syntax_redirect(t_ast *node, t_token *t);
void	syntax_decision_redirect(t_ast *node, t_token *t);
int	syntax_cmd(t_ast *node, t_token *t);
void	parsing(t_ast *tree, t_token *token_header);
void	tree_searching(t_ast *tree);

// utils.c
t_token	*ft_token_dup(t_token *src);
char	*get_tree_type_str(enum e_tree_type tree_type);
