#ifndef MINISHELL_H
# define MINISHELL_H

/*
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <unistd.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
*/

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

/*
 * key: 환경변수의 key
 * value: 환경변수의 value
 * l: 해당 환경변수의 key보다 작은 key의 index
 * r: 해당 환경변수의 key보다 큰 key의 index
 * k_len: key의 길이
 * v_len: value의 길이
 */

int	g_exit_code;

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

typedef enum e_free_level {
	BEF_KEY_SET,
	AFT_KEY_SET
}	t_flevel;

enum e_token {
	T_WORD,
	T_QUOTE = '\'',
	T_DQUOTE = '\"',
	T_DOLLAR = '$',
	T_RE_INPUT,
	T_RE_OUTPUT,
	T_RE_HEREDOC,
	T_RE_APPEND_OUTPUT,
	T_PIPE = '|',
	T_STAR = '*'
};

// basic.c
void				print_error(char *a, char *b, char *c, char *d);
void				*malloc_array(int size, int len);
int					ft_max(int l, int r);
unsigned long long	ft_atoi(const char *str);
void				*ft_realloc(void *ptr, int ptr_size, int new_size);
int					ft_strlen(char *str);
int					ft_strlen_c(char *str, char c);
int					ft_strlcpy(char *str, char *target, int len);
int					ft_strcpy(char *str, char *target);
int					ft_strlcmp(char *l, char *r, int len);
int					ft_strcmp(char *l, char *r);
void				ft_tolower(char **str);
char				*ft_strcat(char *org, char *target);

// free.c
void				free_envs(t_envs *e, t_flevel level);
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

#endif
