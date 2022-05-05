// 사용되는 토큰
enum Token
{
	T_WORD, // 추후에 cmd나 argv가 될 수 있는 아래 특수기호 토큰이 아닌 모든 인자.
	T_QUOTE = '\'',
	T_DQUOTE = '\"',
	T_PIPE = '|',
	T_DOLLAR = '$',
	T_STAR = '*',
	T_RE_INPUT, // <
	T_RE_OUTPUT, // >
	T_RE_HEREDOC, // <<
	T_RE_APPEND_OUTPUT // >>
};

// 토큰화 후 syntaxing 예정.
enum Syntax
{
	S_REDIRECT,
	S_CMD,
	S_PIPE,
	S_ARGV,
};

// 각 토큰들을 linked list로 저장.
typedef struct	s_token
{
	char			*s;
	int				token;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;


int	ft_strlen(char *s);
char	*ft_strndup(char *src, int len);
int	ft_tsplit(t_token *t, char *s);
t_token	*split_one_elem(char *s, int *i);
t_token	*split_quote(char *s, int *i, int *sq_cnt, int *dq_cnt);
t_token	*split_redirect(char *s, int *i);
t_token	*split_word(char *s, int *i);
char	*get_token_str(int token);
