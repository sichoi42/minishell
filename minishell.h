#define OK 0
#define ERROR 1
#define WRONG_ACTION 2

// 사용되는 토큰
enum e_token
{
	T_INIT = 1,
	T_WORD = 1 << 1,
	T_EXIT_CODE = 1<< 2,
	T_RE_INPUT = 1 << 3,
	T_RE_OUTPUT = 1 << 4,
	T_RE_HEREDOC = 1 << 5,
	T_RE_APPEND_OUTPUT = 1 << 6,
	T_PIPE = 1 << 7,
	T_STAR = 1 << 8
};

// 토큰의 type
enum e_type
{
	INIT = 1,
	REDIRECT = 1 << 1,
	PIPE = 1 << 2,
	ARGS = 1 << 3,
	STAR = 1 << 4
}	t_type;

// parse tree의 type
enum e_tree_type
{
	TREE_PIPE, TREE_BUNDLE, TREE_RE, TREE_CMD
}	t_tree_type;

// 각 토큰들을 linked list로 저장.
typedef struct	s_token
{
	char				*s; // 담긴 문자열
	enum e_token		token; // 토큰
	enum e_type			type; // 토큰의 타입
	struct s_token		*next; // 토큰에 연결된 다음 노드
}	t_token;

typedef struct s_ast
{
	t_token				*token; // 토큰 구조체
	struct s_ast		*left; // 왼쪽 자식
	struct s_ast		*right; // 오른쪽 자식
	struct s_ast		*root; // 루트 노드 정보
	enum e_tree_type	tree_type; // 트리의 타입
	int					pipe_cnt; // 파이프 개수
}	t_ast;

typedef	struct s_command
{
	char	*cmd;
	char	*path;
	char	**args;
}	t_command;

typedef struct s_redirect
{
	int		type;
	char	*file_name;
}	t_redirect;

// utils.c
int	ft_strlen(char *s);
char	*ft_strndup(char *src, int len);
char	*ft_strdup(char *src);
int		is_space(char c);

// in tokenize

// utils.c
void	print_token_str(enum e_token token);
void	print_type_str(enum e_type type);
void	print_token_list(t_token *t);
void	free_token(t_token *t);
char	*ft_strnjoin(char *s1, char *s2, int len);
char	*ft_strchr(char *s, char c);
char	*ft_strchr_ig_blsh(char *s, char *c, char **end);

// tokenize.c
char *get_env(char **start, char *end);
char *split_word_in_dollar(char **start, char **end, enum e_token *token);
char *split_word_in_quote(char **start, char **end, enum e_token *token);
void move_quote_end(char **start, char **end);
t_token *split_word(char **start, char **end, enum e_token *token);
void get_redirect_token(char **start, char **end, enum e_token *token);
t_token *split_pipe(char **end, enum e_token *token);
t_token *split_star(char **end, enum e_token *token);
void moving_two_pointers(char **start, char **end);
t_token	*split_redirect(char **start, char **end, enum e_token *token);
int tokenizing(char *line, t_token *t);

// in parsing

// parsing.c
int syntax_pipe(t_ast *node, t_token *t, t_ast *root);
int syntax_bundle(t_ast *node, t_token *t, t_ast *root);
int syntax_redirect(t_ast *node, t_token *t, t_ast *root);
void syntax_decision_redirect(t_ast *node, t_token *t, t_ast *root);
int syntax_cmd(t_ast *node, t_token *t, t_ast *root);
void parsing(t_ast *tree, t_token *token_header);
void execute_something(t_ast *node);
void tree_searching(t_ast *node);
void free_tree(t_ast *node);

// utils.c
t_token	*ft_token_dup(t_token *src);
char	*get_tree_type_str(enum e_tree_type tree_type);
