#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <unistd.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <stdlib.h>

# define SIZE 128

/*
 * key: 환경변수의 key
 * value: 환경변수의 value
 * l: 해당 환경변수의 key보다 작은 key의 index
 * r: 해당 환경변수의 key보다 큰 key의 index
 * k_len: key의 길이
 * v_len: value의 길이
 */

typedef struct s_env {
	char	*key;
	char	*value;
	int		l;
	int		r;
	int		k_len;
	int		v_len;
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
	int		first;
	int		last;
	int		capa;
	int		size;
}	t_envs;

typedef enum e_free_level {
	BEF_KEY_SET,
	AFT_KEY_SET
}	t_flevel;

void	*ft_realloc(void *ptr, int ptr_size, int new_size);
int		ft_strlen(char *str);
int		ft_strlen_c(char *str, char c);
int		ft_strlcpy(char *str, char *target, int len);
int		ft_strlcmp(char *l, char *r, int len);

#endif