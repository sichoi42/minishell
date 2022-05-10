#include "minishell.h"

void	print_error(char *a, char *b, char *c, char *d)
{
	write(2, a, ft_strlen(a));
	if (b != NULL)
	{
		write(2, ": ", 2);
		write(2, b, ft_strlen(b));
		if (c != NULL)
		{
			write(2, ": ", 2);
			write(2, c, ft_strlen(c));
			if (d != NULL)
			{
				write(2, ": ", 2);
				write(2, d, ft_strlen(d));
			}
		}
	}
	write(2, "\n", 1);
}

int	ft_isspace(int c)
{
	if ((c >= 0x09 && c <= 0x0d) || c == ' ')
		return (1);
	else
		return (0);
}

void	check_minus(const char *str, size_t *index, int *is_minus)
{
	if (str[*index] == '-')
	{
		++(*index);
		*is_minus = 1;
	}
	else
	{
		if (str[*index] == '+')
			++(*index);
		*is_minus = 0;
	}
}

unsigned long long ft_atoi(const char *str)
{
	size_t				i;
	int					is_minus;
	unsigned long long	num;

	i = 0;
	while (ft_isspace(str[i]) != 0)
		++i;
	check_minus(str, &i, &is_minus);
	num = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + str[i++] - '0';
		if (num >= OVER_LONG_NUM)
			return (OVER_LONG_NUM);
	}
	if (is_minus == 1 && num > OVER_LONG_NUM)
		return (OVER_LONG_NUM);
	else if (is_minus == 1)
		num *= -1;
	while (ft_isspace(str[i]) != 0)
		++i;
	if (str[i] != '\0')
		return (OVER_LONG_NUM);
	return (num);
}

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
/*
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
*/

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
			free(e->env[i]);
		}
		free(e->envs);
		free(e->env);
	}
}

int	envs_element_alloc(t_envs *e, int i, int max)
{
	if (i == max)
		return (OK);
	e->envs[i]->k_len = ft_strlen_c(e->env[i], '=');
	e->envs[i]->v_len = ft_strlen(e->env[i] + e->envs[i]->k_len + 1);
	e->envs[i]->key = malloc_array(sizeof(char), e->envs[i]->k_len + 1);
	e->envs[i]->value = malloc_array(sizeof(char), e->envs[i]->v_len + 1);
	if (ft_strcmp(e->envs[i]->key, "OLDPWD") == 0)
	{
		free(e->envs[i]->value);
		e->envs[i]->v_len = -1;
	}
	envs_element_alloc(e, i + 1, max);
	return (OK);
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

int	ft_strcpy(char *src, char *target)
{
	int i;

	i = 0;
	while (src[i] != '\0')
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

int	ft_strlcmp(char *l, char *r, int len)
{
	int	i;

	i = -1;
	while (*l != '\0' && ++i < len)
	{
		if (*l != *r)
			break ;
		++l;
		++r;
	}
	if (i == len)
		return (OK);
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
		if (e->envs[i]->v_len != -1)
			printf("%s\n", e->env[i]);
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
		if (e->envs[i]->v_len != -1)
			ft_strlcpy(e->env[i] + e->envs[i]->k_len + 1,
				e->envs[i]->value, e->envs[i]->v_len);
	}
}

static void	ft_envcpy(t_envs *e, char *env[])
{
	int	i;
	int	len;

	e->env = malloc_array(sizeof(char *), (e->size + 1));
	e->env[e->size] = NULL;
	i = -1;
	while (env[++i] != NULL)
	{
		len = ft_strlen(env[i]);
		e->env[i] = malloc_array(sizeof(char), len + 1);
		ft_strcpy(env[i], e->env[i]);
	}
}

int	input_env(t_envs *e, char *env[])
{
	e->size = -1;
	while (env[++(e->size)] != NULL)
		;
	ft_envcpy(e, env);
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
	e->old_pwd = NULL;
	e->pwd = getcwd(NULL, 0);
	if (e->pwd == NULL)
	{
		print_error("bash", strerror(errno), NULL, NULL);
		return (1);
	}
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
	int	k_len;
	int	v_len;

	e->envs[e->size] = malloc_array(sizeof(t_env), 1);
	e->envs[e->size]->k_len = ft_strlen(key);
	if (value == NULL)
		e->envs[e->size]->v_len = -1;
	else
		e->envs[e->size]->v_len = ft_strlen(value);
	k_len = e->envs[e->size]->k_len;
	v_len = e->envs[e->size]->v_len;
	e->envs[e->size]->key = malloc_array(sizeof(char), k_len + 1);
	ft_strcpy(key, e->envs[e->size]->key);
	set_env_lr(e, key, e->size);
	if (v_len != -1)
	{
		e->envs[e->size]->value = malloc_array(sizeof(char), v_len + 1);
		ft_strcpy(value, e->envs[e->size]->value);
	}
	else
		e->envs[e->size]->value = NULL;
	e->size += 1;
	e->envs[e->size] = NULL;
}

void	insert_main_env(t_envs *e, int i, char *key, char *value)
{
	int	k_len;
	int	v_len;

	if (i == -1)
		i = e->size - 1;
	else
		free(e->env[i]);
	k_len = e->envs[i]->k_len;
	v_len = e->envs[i]->v_len;
	if (v_len != -1)
		e->env[i] = malloc_array(sizeof(char), k_len + v_len + 2);
	else
		e->env[i] = malloc_array(sizeof(char), k_len + 1);
	ft_strcpy(e->envs[i]->key, e->env[i]);
	if (v_len != -1)
	{
		e->env[i][k_len] = '=';
		ft_strcpy(e->envs[i]->value, e->env[i] + k_len + 1);
	}
	else
		e->env[i][k_len] = '\0';
	e->env[e->size] = NULL;
}

int	check_key(char *key)
{
	if (*key >= '0' && *key <= '9')
		return (ERROR);
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
			return (ERROR);
		++key;
	}
	return (OK);
}

void	change_exist_env(t_envs *e, int i, char *value)
{
	free(e->envs[i]->value);
	e->envs[i]->v_len = ft_strlen(value);
	e->envs[i]->value = malloc_array(sizeof(char), e->envs[i]->v_len + 1);
	ft_strcpy(value, e->envs[i]->value);
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
		if (e->capa == e->size + 2)
		{
			e->capa *= 2;
			e->envs = (t_env **)ft_realloc(e->envs, sizeof(t_env *) * e->size,
					sizeof(t_env *) * e->capa);
			e->env = (char **)ft_realloc(e->env, sizeof(char *) * e->size,
					sizeof(char *) * e->capa);
		}
		insert_new_env(e, key, value);
	}
	else
	{
		if (value == NULL)
			return (0);
		change_exist_env(e, index, value);
	}
	insert_main_env(e, index, key, value);
	return (0);
}

/*
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
*/

void	delete_main_env(t_envs *e, int i, char *key)
{
	free(e->env[i]);
	if (i != e->size)
		e->env[i] = e->env[e->size];
	e->env[e->size] = NULL;
}

int	delete_env(t_envs *e, char *key)
{
	int	index;

	if (ft_strcmp(key, "_") == 0)
		return (0);
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
	e->envs[e->size] = NULL;
	delete_main_env(e, index, key);
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
		print_error("bash", strerror(errno), NULL, NULL);
		//printf("bash: %s\n", strerror(errno));
		exit(1);
	}
	return (OK);
}

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
		print_error("bash", TEMP_FILE, strerror(errno), NULL);
		//printf("bash: %s: %s\n", TEMP_FILE, strerror(errno));
		return (WRONG_ACTION);
	}
	unlink(TEMP_FILE);
	return (dup_check(*fd, STDIN_FILENO));
}

int	red_in(char *f_name)
{
	int fd;

	fd = open(f_name, O_RDWR);
	if (fd == -1)
		print_error("bash", f_name, strerror(errno), NULL);
		//printf("bash: %s: %s\n", f_name, strerror(errno));
	else
	{
		dup_check(fd, STDIN_FILENO);
		close(fd);
		return (OK);
	}
	return (WRONG_ACTION);
}

int	red_out(char *f_name)
{
	int fd;

	fd = open(f_name, O_RDWR | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
		print_error("bash", f_name, strerror(errno), NULL);
		//printf("bash: %s: %s\n", f_name, strerror(errno));
	else
	{
		dup_check(fd, STDOUT_FILENO);
		close(fd);
		return (OK);
	}
	return (WRONG_ACTION);
}

int	red_append(char *f_name)
{
	int fd;

	fd = open(f_name, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd == -1)
		print_error("bash", f_name, strerror(errno), NULL);
		//printf("bash: %s: %s\n", f_name, strerror(errno));
	else
	{
		dup_check(fd, STDOUT_FILENO);
		close(fd);
		return (OK);
	}
	return (WRONG_ACTION);
}

/*
 * 오픈 하는 건 완료
 * 함수 완료 후 에러 발생시 나머지 리다이렉션에 대한 처리는 따로 필요
 * red 값은 무조건 4가지 중 하나만 들어온다는 가정으로 만듬.
 * append 모두 처리 필요, fd값 하나로 정리 필요(fclose해야함.)
 * here_doc의 경우 f_name에 ".이름" 방식으로 이름 넣기.
 */
// 리다이렉션 오픈하는 함수 + heredoc 실행까지
int	red_open_file(enum e_token t, char *f_name)
{
	int	fd;

	if (t == T_RE_INPUT)
		return (red_in(f_name));
	else if (t == T_RE_APPEND_OUTPUT)
		return (red_append(f_name));
	else if (t == T_RE_OUTPUT)
		return (red_out(f_name));
	else if (t == T_RE_HEREDOC)
	{
		fd = open(TEMP_FILE, O_RDWR | O_CREAT | O_TRUNC, 0664);
		if (fd == -1)
		{
			print_error("bash", TEMP_FILE, strerror(errno), NULL);
			//printf("bash: %s: %s\n", TEMP_FILE, strerror(errno));
			return (WRONG_ACTION);
		}
		return (red_here_doc(&fd, f_name));
	}
	return (WRONG_ACTION);
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

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path ==  NULL)
	{
		print_error("bash", "pwd", strerror(errno), NULL);
		//printf("bash: %s: %s\n", "pwd", strerror(errno));
		return (ERROR);
	}
	else
		printf("%s\n", path);
	free(path);
	return (OK);
}

int	ft_exit(t_oper *o)
{
	unsigned long long	num;

	if (o->opers[1] != NULL)
	{
		num = ft_atoi(o->opers[1]);
		if (num == OVER_LONG_NUM)
		{
			print_error("bash", "exit", o->opers[1], NUMERIC_ERROR);
			//printf("bash: exit: %s: %s\n", o->opers[i], NUMERIC_ERROR);
			exit(255);
		}
		if (o->opers[2] != NULL)
		{
			print_error("bash", "exit", ARG_NUM_ERROR, NULL);
			//printf("bash: exit: %s\n", ARG_NUM_ERROR);
			return (1);
		}
		// single command면.
		printf("exit\n");
		printf("%lld\n", num % 256);
		exit(num % 256);
	}
	// single command면.
	printf("exit\n");
	exit(OK);
}

int	ft_env(t_envs *e)
{
	// 이 부분도 결정해야함. bash랑 똑같이 할지, 아니면..?
	insert_env(e, "_", "/usr/bin/env");
	print_env(e);
	return (OK);
}

int	check_key_error(char *oper, char *arg)
{
	int		len;
	char	*err_arg;

	len = ft_strlen(arg);
	err_arg = malloc_array(sizeof(char), len + 3);
	err_arg[0] = '`';
	ft_strcpy(arg, err_arg + 1);
	err_arg[len + 1] = '\'';
	err_arg[len + 2] = '\0';
	print_error("bash", oper, err_arg, NOT_VALID_ERROR);
	return (ERROR);
}

int	ft_unset(t_oper *o, t_envs *e)
{
	int		i;
	int		len;
	int		rtn;
	char	*e_oper;

	i = 0;
	rtn = OK;
	while (o->opers[++i] != NULL)
	{
		if (check_key(o->opers[i]) == ERROR)
			rtn = check_key_error("unset", o->opers[i]);
		else
		{
			delete_env(e, o->opers[i]);
			if (ft_strcmp(o->opers[i], "PWD") == 0)
				e->pwd[0] = '\0';
		}
	}
	return (rtn);
}

/*
 * 따옴표 처리하는 부분을 추가해줘야 한다.
 */

static void	make_key_value(char *arg, char **key, char **value)
{
	int	i;
	int	k_len;
	int	v_len;

	k_len = ft_strlen_c(arg, '=');
	*key = malloc_array(sizeof(char), k_len + 1);
	ft_strlcpy(arg, *key, k_len);
	i = -1;
	while (arg[++i] != '\0')
		if (arg[i] == '=')
			break ;
	if (arg[i] != '\0')
	{
		v_len = ft_strlen(arg + k_len + 1);
		*value = malloc_array(sizeof(char), v_len + 1);
		ft_strcpy(arg + k_len + 1, *value);
		return ;
	}
	v_len = -1;
	*value = NULL;
}

// 따옴표 들어왔을 때 처리는 따로 해줘야함.
// 이건 입력이 실제로 어떻게 들어오는지 확인하면서 수정하자.
int	ft_export(t_oper *o, t_envs *e)
{
	int		i;
	int		rtn;
	char	*e_oper;
	char	*key;
	char	*value;

	rtn = OK;
	i = 0;
	while (o->opers[++i] != NULL)
	{
		make_key_value(o->opers[i], &key, &value);
		if (check_key(key) == ERROR)
			rtn = check_key_error("export", o->opers[i]);
		else
			insert_env(e, key, value);
	}
	if (i == 1)
		print_export(e);
	free(key);
	free(value);
	return (rtn);
}

char	*ft_strcat(char *org, char *target)
{
	int	i;

	i = -1;
	while (target[++i] != '\0')
		;
	while (*org != '\0')
		target[i++] = *org++;
	target[i] = '\0';
	return (target);
}

static int	check_echo_option(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
	{
		while (arg[++i] == 'n')
			;
		if (arg[i] == '\0')
			return (OK);
	}
	return (1);
}

int	ft_echo(t_oper *o)
{
	int	i;
	int	first;
	int	option;

	option = 0;
	i = 0;
	while (o->opers[++i] != NULL)
		if (check_echo_option(o->opers[i]) != OK)
			break ;
		else if (option == 0)
			option = 1;
	first = 1;
	while (o->opers[i] != NULL)
	{
		if (first++ != 1)
			printf(" ");
		printf("%s", o->opers[i++]);
	}
	if (option == 0)
		printf("\n");
	return (OK);
}

static int	set_pwd(t_envs *e, char *pwd)
{
	free(e->old_pwd);
	e->old_pwd = e->pwd;
	e->pwd = malloc_array(sizeof(char), ft_strlen(pwd) + 1);
	ft_strcpy(pwd, e->pwd);
	if (search_env(e, "PWD") != -1)
		insert_env(e, "PWD", pwd);
	if (search_env(e, "OLDPWD") != -1)
		insert_env(e, "OLDPWD", e->old_pwd);
	return (OK);
}

int	ft_cd(t_oper *o, t_envs *e)
{
	char	*temp;

	if (o->opers[1] != NULL)
	{
		if (chdir(o->opers[1]) != OK)
		{
			print_error("bash", "cd", o->opers[1], strerror(errno));
			return (ERROR);
		}
		return (set_pwd(e, o->opers[1]));
	}
	temp = get_env_value(e, "HOME");
	if (temp == NULL)
	{
		print_error("bash", "cd", "HOME not set", NULL);
		return (ERROR);
	}
	if (chdir(temp) != OK)
	{
		print_error("bash", "cd", o->opers[1], strerror(errno));
		return (ERROR);
	}
	return (set_pwd(e, temp));
}

/* 리턴값을 exit_code로 넣을 것인지,
 * g_exit_code에 값은 넣고, return은 check의 용도로만 쓸것인지 정해야함.
 */

int	built_in_check(t_oper *o, t_envs *e)
{
	int	len;

	len = ft_strlen(o->opers[0]);
	if (len == 6 && ft_strcmp("export", o->opers[0]) == 0)
		return (ft_export(o, e));
	else if (len == 5 && ft_strcmp("unset", o->opers[0]) == 0)
		return (ft_unset(o, e));
	else if (len == 4 && ft_strcmp("exit", o->opers[0]) == 0)
		return (ft_exit(o));
	else
	{
		ft_tolower(&(o->opers[0]));
		if (len == 4 && ft_strcmp("echo", o->opers[0]) == 0)
			return (ft_echo(o));
		else if (len == 2 && ft_strcmp("cd", o->opers[0]) == 0)
			return (ft_cd(o, e));
		else if (len == 3 && ft_strcmp("pwd", o->opers[0]) == 0)
			return (ft_pwd());
		else if (len == 3 && ft_strcmp("env", o->opers[0]) == 0)
			return (ft_env(e));
		else
			return (-1);
	}
	return (OK);
}

void	init_pipe(int *pipe_fd)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
}

int	make_pipe(int *p_fd)
{
	int	rtn;

	rtn = pipe(p_fd);
	if (rtn == -1)
	{
		printf("bash: %s\n", strerror(errno));
		exit(1);
	}
	dup_check(p_fd[1], STDOUT_FILENO);
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
	int		pipe_fd[2];
	int		status;

	if (input_env(&e, env) == 1)
		return (1);
	// 여기서부터 redirection 부분.

	/* >> 시스템 함수 실행시 pipe close()할 때를 위해 init() */
	//init_pipe(pipe_fd);
	/* >> 만약 pipe가 있으면 미리 여기서 pipe 생성 후 out redirection 연결*/
	//make_pipe(pipe_fd);
	/* >> pipe가 없으면 stdout 다시 제대로 연결 */
	//dup_check(STDOUT_FILENO, STDOUT_FILENO);

	/* >> redirection open && dup2 && close(fd) */
	//if (red_open_file(T_RE_HEREDOC, "a") == WRONG_ACTION)
		/*error check 후에 종료 또는 다음 pipe 실행*/

	find_path(&(p.paths), &(p.max_len));
	input_oper = argv[1];
	o.oper_path = make_oper(&o.opers, p.max_len, p.paths, input_oper);
	
	// 파이프 유무 체크 후 확인.
	// 파이프가 있으면 cd 도 동작 안함.(이동하지 않음)
	// 즉 파이프가 있으면 무조건 fork해서 실행된다.
	
	// 아래 함수 부터 수정하면 됨.
	/*===========================================================*/
	/*
	 * if (파이프 있으면)
	 * 		자식 생성 함수
	 * else
	 * 		if 빌트인 함수면
	 * 			빌트인 함수 실행
	 * 		else
	 * 			시스템 함수 실행
	 */
	/*===========================================================*/
	if (built_in_check(&o, &e) == -1)
	{
		if (o.oper_path == NULL)
			printf("bash: %s: %s\n", o.opers[0], COMMAND_ERROR); // exit:127
		else
			exe_oper(&o, pipe_fd, e.env);
	}
	/*===========================================================*/
	while (waitpid(-1, &status, 0) >= 0)
		;
	// 만약 다음 파이프가 없고 status == 11 이라면 아래의 값 출력
		//printf("Segmentation fault: %d\n", status);
		//exit code: 128+11=139
	free_double_char(o.opers);// 매 번들마다.
	free(o.oper_path);//매 번들마다.

	free_envs(&e, AFT_KEY_SET);//bash 종료시에.
	free_double_char(p.paths);// bash 종료시에.
	return (0);
}
