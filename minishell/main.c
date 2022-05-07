#include "minishell.h"

void	*ft_realloc(void *ptr, int ptr_size, int new_size)
{
	char	*temp;
	char	*cp_ptr;
	int		i;

	temp = (char *)malloc(new_size);
	if (temp == NULL)
		exit(1);
	cp_ptr = (char *)ptr;
	i = -1;
	while (++i < ptr_size)
		temp[i] = cp_ptr[i];
	free(ptr);
	return (temp);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (*str++ != '\0')
		++len;
	return (len);
}

int	ft_strlen_c(char *str, char c)
{
	int	len;

	len = 0;
	while (*str != '\0' && *str != c)
	{
		++len;
		++str;
	}
	return (len);
}

int	envs_alloc(void **ptr, int size, int i, int max)
{
	if (i == max)
		return (0);
	ptr[i] = malloc(size);
	if (ptr == NULL)
		return (1);
	if (envs_alloc(ptr, size, i + 1, max) == 1)
	{
		free(ptr[i]);
		if (i == 0)
			free(ptr);
		return (1);
	}
	return (0);
}

void	free_envs(t_envs *e, t_flevel level)
{
	int	i;

	i = -1;
	if (level == BEF_KEY_SET)
	{
		while (++i < e->size)
			free(e->envs[i]);
		free(e->envs);
	}
	else if (level == AFT_KEY_SET)
	{
		while (++i < e->size)
		{
			free(e->envs[i]->key);
			free(e->envs[i]->value);
			free(e->envs[i]);
		}
		free(e->envs);
	}
}

int	envs_element_alloc(t_envs *e, int i, int max)
{
	if (i == max)
		return (0);
	e->envs[i]->k_len = ft_strlen_c(e->env[i], '=');
	e->envs[i]->v_len = ft_strlen(e->env[i] + e->envs[i]->k_len + 1);
	e->envs[i]->key = malloc(sizeof(char) * (e->envs[i]->k_len + 1));
	if (e->envs[i]->key == NULL)
		return (1);
	e->envs[i]->value = malloc(sizeof(char) * (e->envs[i]->v_len + 1));
	if (e->envs[i]->value == NULL)
	{
		free(e->envs[i]->key);
		return (1);
	}
	if (envs_element_alloc(e, i + 1, max) == 1)
	{
		free(e->envs[i]->key);
		free(e->envs[i]->value);
		if (i == 0)
			free_envs(e, BEF_KEY_SET);
		return (1);
	}
	return (0);
}

int	ft_strlcpy(char *src, char *target, int len)
{
	int i;

	i = 0;
	while (src[i] != '\0' && i < len)
	{
		target[i] = src[i];
		++i;
	}
	target[i] = '\0';
	return (i);
}

/*
 * 왼쪽이 더 크면 양수
 * 오른쪽이 더 크면 음수
 * 같으면 0
 */

int	ft_strcmp(char *l, char *r)
{
	while (*l != '\0')
	{
		if (*l != *r)
			break ;
		++l;
		++r;
	}
	return (*l - *r);
}

void	print_export(t_envs *e)
{
	t_env	*env;

	env = e->first;
	while (env != NULL)
	{
		if (ft_strcmp("_", env->key) != 0)
		{
			if (env->v_len != -1)
				printf("declare -x %s=\"%s\"\n", env->key, env->value);
			else
				printf("declare -x %s\n", env->key);
		}
		env = env->r;
	}
}

void	print_env(t_envs *e)
{
	int i;

	i = -1;
	while (++i < e->size)
	{
		if (e->envs[i]->v_len != -1)
			printf("%s=%s\n", e->envs[i]->key, e->envs[i]->value);
	}
}

void	find_env_r(t_envs *e, char *key, int last)
{
	int	i;
	t_env	*bef;

	i = -1;
	while (++i < last && e->envs[last]->r == NULL)
	{
		if (ft_strcmp(e->envs[i]->key, key) > 0)
		{
			bef = e->envs[i]->l;
			if (bef != NULL && ft_strcmp(bef->key, key) < 0)
			{
				e->envs[last]->r = e->envs[i];
				e->envs[last]->l = bef;
				bef->r = e->envs[last];
				e->envs[i]->l = e->envs[last];
			}
			else if (bef == NULL)
			{
				e->envs[last]->r = e->envs[i];
				e->envs[i]->l = e->envs[last];
			}
		}
	}
}

void	find_env_l(t_envs *e, char *key, int last)
{
	int	i;
	t_env	*next;

	i = -1;
	while (++i < last && e->envs[last]->l == NULL)
	{
		if (ft_strcmp(e->envs[i]->key, key) < 0)
		{
			next = e->envs[i]->r;
			if (next != NULL && ft_strcmp(next->key, key) > 0)
			{
				e->envs[last]->r = next;
				e->envs[last]->l = e->envs[i];
				next->l = e->envs[last];
				e->envs[i]->r = e->envs[last];
			}
			else if (next == NULL)
			{
				e->envs[last]->l = e->envs[i];
				e->envs[i]->r = e->envs[last];
			}
		}
	}
}

/*
 * index는 새로 입력된 envs에 대한 index
 * key는 해당 index의 key값(새로 입력한 key값)
 * 아래 함수는 새로 입력된 환경변수의 순서를 결정하기 위해
 * 해당 key 보다 큰 값의 인덱스(r), 작은 인덱스(l)을 찾아주는 함수
 */

void	set_env_lr(t_envs *e, char *key, int index)
{
	e->envs[index]->l = NULL;
	e->envs[index]->r = NULL;
	find_env_r(e, key, index);
	find_env_l(e, key, index);
	if (e->envs[index]->l == NULL)
		e->first = e->envs[index];
	if (e->envs[index]->r == NULL)
		e->last = e->envs[index];
}

void	init_envs(t_envs *e)
{
	int	i;

	i = -1;
	while (++i < e->size)
	{
		ft_strlcpy(e->env[i], e->envs[i]->key, e->envs[i]->k_len);
		set_env_lr(e, e->envs[i]->key, i);
		ft_strlcpy(e->env[i] + e->envs[i]->k_len + 1,
			e->envs[i]->value, e->envs[i]->v_len);
	}
}

int	input_env(t_envs *e, char *env[])
{
	e->env = env;
	e->size = -1;
	while (env[++(e->size)] != NULL)
		;
	e->capa = SIZE;
	while (e->capa <= e->size)
		e->capa *= 2;
	e->envs = malloc(sizeof(t_env *) * e->capa);
	if (e->envs == NULL)
		return (1);
	if (envs_alloc((void **)e->envs, sizeof(t_env), 0, e->size) == 1)
		return (1);
	if (envs_element_alloc(e, 0, e->size) == 1)
		return (1);
	init_envs(e);
	return (0);
}

int	search_env(t_envs *e, char *key)
{
	int	i;

	i = -1;
	while (++i < e->size)
	{
		if (ft_strcmp(key, e->envs[i]->key) == 0)
			return (i);
	}
	return (-1);
}

char	*get_env_value(t_envs *e, char *key)
{
	int	i;

	i = search_env(e, key);
	if (i == -1)
		return (NULL);
	return (e->envs[i]->value);
}

static void	insert_new_env(t_envs *e, char *key, char *value)
{
	e->envs[e->size] = malloc(sizeof(t_env));
	if (e->envs[e->size] == NULL)
		exit(1);
	e->envs[e->size]->key = key;
	e->envs[e->size]->k_len = ft_strlen(key);
	set_env_lr(e, key, e->size);
	e->envs[e->size]->value = value;
	if (value == NULL)
		e->envs[e->size]->v_len = -1;
	else
		e->envs[e->size]->v_len = ft_strlen(value);
	e->size += 1;
}

int	check_key(char *key)
{
	if (*key >= '0' && *key <= '9')
		return (1);
	while (*key != '\0')
	{
		if (*key == '_')
			;
		else if (*key >= 'a' && *key <= 'z')
			;
		else if (*key >= 'A' && *key <= 'Z')
			;
		else if (*key >= '0' && *key <= '9')
			;
		else
			return (1);
		++key;
	}
	return (0);
}

/*
 * insert_env에 들어오기 전 key와 value를 check해야한다.check_key() 함수 사용.
 * key, value는 malloc된 값이 들어온다고 가정
 */

int	insert_env(t_envs *e, char *key, char *value)
{
	int	index;

	index = search_env(e, key);
	if (index == -1)
	{
		if (e->capa == e->size + 1)
		{
			e->capa *= 2;
			e->envs = (t_env **)ft_realloc(e->envs, sizeof(t_env *) * e->size,
					e->capa);
		}
		insert_new_env(e, key, value);
	}
	else
	{
		if (value == NULL)
			return (0);
		free(e->envs[index]->value);
		e->envs[index]->value = value;
		e->envs[index]->v_len = ft_strlen(value);
	}
	return (0);
}

int	delete_env(t_envs *e, char *key)
{
	int	index;

	index = search_env(e, key);
	if (index == -1)
		return (1);
	free(e->envs[index]->key);
	free(e->envs[index]->value);
	if (e->envs[index]->l != NULL)
		e->envs[index]->l->r = e->envs[index]->r;
	else
		e->first = e->envs[index]->r;
	if (e->envs[index]->r != NULL)
		e->envs[index]->r->l = e->envs[index]->l;
	else
		e->last = e->envs[index]->l;
	free(e->envs[index]);
	if (index != e->size - 1)
		e->envs[index] = e->envs[e->size - 1];
	e->size -= 1;
	return (0);
}

// dup2와 error check까지
int	dup_check(int fd_l, int fd_r)
{
	int	rtn;

	rtn = dup2(fd_l, fd_r);
	if (rtn == -1)
	{
		// 추후 struct 정해지면 파일이름 추가해서 에러 출력
		printf("bash: %s\n", strerror(errno));
		exit(1);
	}
	return (OK);
}

// heredoc 실행
int	red_here_doc(int *fd, char *limit)
{
	char	*str;

	while (1)
	{
		rl_replace_line("", 1);
		str = readline("> ");
		if (ft_strcmp(str, limit) == 0)
			break ;
		write(*fd, str, ft_strlen(str));
	}
	close(*fd);
	*fd = open(TEMP_FILE, O_RDWR);
	if (*fd == -1)
	{
		printf("bash: %s: %s\n", TEMP_FILE, strerror(errno));
		return (WRONG_ACTION);
	}
	unlink(TEMP_FILE);
	return (dup_check(*fd, STDIN_FILENO));
}

/*
 * 오픈 하는 건 완료
 * 함수 완료 후 에러 발생시 나머지 리다이렉션에 대한 처리는 따로 필요
 * red 값은 무조건 4가지 중 하나만 들어온다는 가정으로 만듬.
 * append 모두 처리 필요, fd값 하나로 정리 필요(fclose해야함.)
 * here_doc의 경우 f_name에 ".이름" 방식으로 이름 넣기.
 */
// 리다이렉션 오픈하는 함수 + heredoc 실행까지
int	red_open_file(int *fd, enum e_token t, char *f_name)
{
	if (t == T_RE_INPUT)
		*fd = open(f_name, O_RDWR);
	else if (t == T_RE_APPEND_OUTPUT)
		*fd = open(f_name, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (t == T_RE_OUTPUT)
		*fd = open(f_name, O_RDWR | O_CREAT | O_TRUNC, 0664);
	else if (t == T_RE_HEREDOC)
	{
		*fd = open(TEMP_FILE, O_RDWR | O_CREAT | O_TRUNC, 0664);
		if (*fd == -1)
		{
			printf("bash: %s: %s\n", TEMP_FILE, strerror(errno));
			return (WRONG_ACTION);
		}
		return (red_here_doc(fd, f_name));
	}
	if (*fd == -1)
		printf("bash: %s: %s\n", f_name, strerror(errno));
	return (OK);
}

void	free_double_char(char **dptr)
{
	char **temp;

	temp = dptr;
	while (*temp != NULL)
	{
		free(*temp);
		temp += 1;
	}
	free(dptr);
}

void	ft_tolower(char **str)
{
	int	i;
	int	diff;

	i = -1;
	diff = 'a' - 'A';
	while ((*str)[++i] != '\0')
	{
		if ((*str)[i] >= 'A' && (*str)[i] <= 'Z')
			(*str)[i] += diff;
	}
}

int	built_in_check(char *oper)
{
	if (ft_strcmp("export", oper) == 0)
		;
	else if (ft_strcmp("unset", oper) == 0)
		;
	else if (ft_strcmp("exit", oper) == 0)
		;
	else
	{
		ft_tolower(&oper);
	}
	return (0);
}

/*
 * redirection처리시 남은 부분. 
 * 1. 구조체(이름모름)에서 받은 값을 red_open_file, red_pre_execute 잘 처리해주기
 * 2. 에러 제대로 출력.
 * 3. 출력값 redirection 처리
 * 4. redirection 처리하면서 만든 fd들 close해줘야함. 
 * 4-1. 마지막 in 아니면 바로 close. here_doc & in은 execute 후에 바로 close. 
 * 4-2. 마지막 out 아니면 바로 close.
 * 5. stdout 따로 저장해줘야할까? 그냥 쓰면 될까?
 *
 * 출력은 3가지 경우로 나눔.
 * 우선 출력용 fd 하나를 두고, 그 fd값에 red~ 또는 pipe를 넣어주고
 * stdout이랑 연결
 * 파이프와 red~가 없다면 dup_check 없이 바로 stdout
 */

int main(int argc, char *argv[], char *env[])
{
	t_envs	e;
	int		fd;
	int		i;
	t_paths	p;
	t_oper	o;
	char	*input_oper;

	if (input_env(&e, env) == 1)
		return (1);
	// 여기서부터 redirection 부분.
	//if (red_open_file(&fd, T_RE_HEREDOC, "a") == -1)
		/*error check*/
	// 마지막 (in or heredoc 와 out or append) 를 dup2 해줘야함.
	// 재귀함수를 이용해서 마지막 경우에만 dup2를 하고, 나머지는 걍 close();
	// dup2 이후에 모든 fd close 해주는 함수 필요.
	// 파이프 처리시에 out쪽 red~ 있으면 파이프로 넘겨줄 값 없음.
	// out 없으면 여기서 나온 값을 넘겨줌.
	// 새로 pipe시작하면 stdin 값을 원상복귀 시켜줘야할 듯?(stdin 버퍼 지우기)

	find_path(&(p.paths), &(p.max_len));
	input_oper = argv[1];
	o.oper_path = make_oper(&o.opers, p.max_len, p.paths, input_oper);
	printf("%s\n", o.oper_path);
	built_in_check(o.opers[0]);
	// built-in 함수 체크
	// built-in 실행
	// 시스템 함수 실행
	// 파이프 유무 체크 후 확인.
	free_double_char(o.opers);// 매 번들마다.
	free(o.oper_path);//매 번들마다.

	free_envs(&e, AFT_KEY_SET);//bash 종료시에.
	free_double_char(p.paths);// bash 종료시에.
	return (0);
}
