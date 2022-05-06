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

int main(int argc, char *argv[], char *env[])
{
	t_envs	e;

	if (input_env(&e, env) == 1)
		return (1);
	free_envs(&e, AFT_KEY_SET);
	return (0);
}
