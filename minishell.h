#ifndef MINISHELL_H
# define MINISHELL_H

# define SIZE 2048

# define TEMP_FILE "/tmp/.heredoc_temp"

# define OK 0
# define ERROR 1
# define WRONG_ACTION 2

# define COMMAND_ERROR "command not found"
# define ARG_NUM_ERROR "too many arguments"
# define NUMERIC_ERROR "numeric argument required"
# define NOT_VALID_ERROR "not a valid identifier"

# define OVER_LONG_NUM 9223372036854775808UL

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
	int					argc;
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

/*
 * key: 환경변수의 key
 * value: 환경변수의 value
 * l: 해당 환경변수의 key보다 작은 key의 index
 * r: 해당 환경변수의 key보다 큰 key의 index
 * k_len: key의 길이
 * v_len: value의 길이
 */
typedef struct s_env {
	char			*key;
	char			*value;
	struct s_env	*l;
	struct s_env	*r;
	int				k_len;
	int				v_len;
}	t_env;

/*
 * env는 main에서 받아온 것 그대로 저장.
 * envs는 key, value로 나눠놓은 값
 * first는 제일 작은 key값을 가진 envs의 index.
 * alst는 제일 큰 key값을 가진 envs의 index.
 * capa는 envs를 할당해놓은 크기(sizeof(t_env *)를 몇 개 할당했는지)
 * size는 envs 내부에 sizeof(t_env)를 몇 개 할당했는지.(저장된 환경변수 크기)
 *
 * capa는 여유롭게 잡아놓음.
 * size는 envs에 들어가있는 환경변수의 수.
 * envs는 size만큼 할당되어 있음.
 * 따라서 새로 값을 추가할 때
 *  size += 1 해주고 capa 보다 작은지 확인하고, capa와 같으면 ft_realloc
 *  그 후에 envs[새로운 인덱스]에 값 할당.
 */
typedef struct s_envs {
	char	**env;
	t_env	**envs;
	t_env	*first;
	t_env	*last;
	int		capa;
	int		size;
	char	*pwd;
	char	*old_pwd;
}	t_envs;

typedef struct s_paths {
	char	**paths;
	int		max_len;
}	t_paths;

typedef struct s_oper {
	char	*oper_path;
	char	**opers;
}	t_oper;

int	g_exit_code;


// sichoi
// ==================================================================
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
char *get_env(char **start, char *end, t_envs *e);
char *split_word_in_dollar(char **start, char **end, enum e_token *token, t_envs *e);
char *split_word_in_quote(char **start, char **end, enum e_token *token, t_envs *e);
void move_quote_end(char **start, char **end);
t_token *split_word(char **start, char **end, enum e_token *token, t_envs *e);
void get_redirect_token(char **start, char **end, enum e_token *token);
t_token *split_pipe(char **end, enum e_token *token);
t_token *split_star(char **end, enum e_token *token);
void moving_two_pointers(char **start, char **end);
t_token *split_redirect(char **start, char **end, enum e_token *token, t_envs *e);
int tokenizing(char *line, t_token *t, t_envs *e);

// in parsing

// parsing.c
int syntax_pipe(t_ast *node, t_token *t, t_ast *root);
int syntax_bundle(t_ast *node, t_token *t, t_ast *root);
int syntax_redirect(t_ast *node, t_token *t, t_ast *root);
void syntax_decision_redirect(t_ast *node, t_token *t, t_ast *root);
int syntax_cmd(t_ast *node, t_token *t, t_ast *root);
void parsing(t_ast *tree, t_token *token_header, t_envs *e);
void execute_something(t_ast *node, t_envs *e);
void tree_searching(t_ast *node, t_envs *e);
void free_tree(t_ast *node);

// utils.c
t_token	*ft_token_dup(t_token *src);
char	*get_tree_type_str(enum e_tree_type tree_type);
// ==================================================================





// swi
// ==================================================================
// basic.c
void				print_error(char *a, char *b, char *c, char *d);
void				*malloc_array(int size, int len);
int					ft_max(int l, int r);
unsigned long long	ft_atoi(const char *str);
void				*ft_realloc(void *ptr, int ptr_size, int new_size);
int					ft_strlen_c(char *str, char c);
int					ft_strlcpy(char *str, char *target, int len);
int					ft_strcpy(char *str, char *target);
int					ft_strlcmp(char *l, char *r, int len);
int					ft_strcmp(char *l, char *r);
void				ft_tolower(char **str);
char				*ft_strcat(char *org, char *target);

// free.c
void				free_envs(t_envs *e);
void				free_double_char(char **dptr);

// input_env.c
int					input_env(t_envs *e, char *env[]);

// builtin.c
int					ft_pwd(void);
int					ft_exit(t_oper *o);
int					ft_env(t_envs *e);
int					ft_unset(t_oper *o, t_envs *e);
int					ft_export(t_oper *o, t_envs *e);
int					ft_echo(t_oper *o);
int					ft_cd(t_oper *o, t_envs *e);
int					built_in_check(t_oper *o, t_envs *e);

// env.c
void				print_export(t_envs *e);
void				print_env(t_envs *e);
void				set_env_lr(t_envs *e, char *key, int index);
int					insert_env(t_envs *e, char *key, char *value);
int					search_env(t_envs *e, char *key);
char				*get_env_value(t_envs *e, char *key);
int					delete_env(t_envs *e, char *key);

// redirect.c
int					dup_check(int fd_l, int fd_r);
int					red_open_file(enum e_token t, char *f_name);
void				init_pipe(int *pipe_fd);
int					make_pipe(int *p_fd);

// pipe.c
void				count_words_alloc(char ***paths, char *org, char div[], int *count);
void				count_word_alloc(char *org, char **words, char *div, int *max_len);
void				fill_array(char *org, char **words, char *div, int count);
char				*make_oper_path(char *oper_path, char *path, char *oper);
void				find_path(char ***paths, int *max_path);
char				*make_oper(char ***opers, int max_path, char **paths, char *argv);
void				exe_oper(t_oper *o, int *pipe_fd, char *envp[]);
// ==================================================================



#endif
