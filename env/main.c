#include "minishell.h"

void	*ft_realloc(void *ptr, int ptr_size, int new_size)
{
	char	*temp;
	char	*cp_ptr;
	int		i;

	temp = (char *)malloc(new_size);
	if (temp == NULL)
		return (NULL);
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

int	ft_strlcmp(char *l, char *r, int len)
{
	int	i;

	if (len == -1)
	{
		while (*l != '\0')
		{
			if (*l++ != *r++)
				break ;
		}
		return (*l - *r);
	}
	else
	{
		i = -1;
		while (++i < len && *l != '\0')
		{
			if (*l++ != *r++)
				break ;
		}
		if (i == len)
			return (0);
		return (*l - *r);
	}
}

void	find_env_r(t_envs *e, char *key, int last)
{
	int	i;
	int	bef;

	i = -1;
	while (++i < last && e->envs[last]->r == -1)
	{
		if (ft_strlcmp(e->envs[i]->key, key, -1) > 0)
		{
			bef = e->envs[i]->l;
			if (bef != -1 && ft_strlcmp(e->envs[bef]->key, key, -1) < 0)
			{
				e->envs[last]->r = i;
				e->envs[last]->l = bef;
				e->envs[bef]->r = last;
				e->envs[i]->l = last;
			}
			else if (bef == -1)
			{
				e->envs[last]->r = i;
				e->envs[i]->l = last;
			}
		}
	}
}

void	find_env_l(t_envs *e, char *key, int last)
{
	int	i;
	int	next;

	i = -1;
	while (++i < last && e->envs[last]->l == -1)
	{
		if (ft_strlcmp(e->envs[i]->key, key, -1) < 0)
		{
			next = e->envs[i]->l;
			if (next != -1 && ft_strlcmp(e->envs[next]->key, key, -1) > 0)
			{
				e->envs[last]->r = next;
				e->envs[last]->l = i;
				e->envs[next]->l = last;
				e->envs[i]->r = last;
			}
			else if (next == -1)
			{
				e->envs[last]->l = i;
				e->envs[i]->r = last;
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
	e->envs[index]->l = -1;
	e->envs[index]->r = -1;
	find_env_r(e, e->envs[index]->key, index);
	find_env_l(e, e->envs[index]->key, index);
}

void	set_envs(t_envs *e)
{
	int	i;
	int	j;
	int	n;
	int	b;
	char	*tmp;

	i = -1;
	while (++i < e->size)
	{
		ft_strlcpy(e->env[i], e->envs[i]->key, e->envs[i]->k_len);
		set_env_lr(e, e->envs[i]->key, i);
		ft_strlcpy(e->env[i] + e->envs[i]->k_len + 2,
			e->envs[i]->value, e->envs[i]->v_len);
	}
	// 제일 작은 환경변수 e->first
	// 제일 큰 환경변수 e->last
	// 를 찾아주는 로직 작성해야함.
	// + 환경변수 출력 함수를 만들어서 위의 로직 검증해야함.
	// 아직 순서를 맞춰주는 로직은 확인하지 못함.
}

int	input_env(t_envs *e, char *env[])
{
	int	i;

	e->env = env;
	e->size = -1;
	while (env[++(e->size)] != NULL)
		;
	e->capa = SIZE;
	while (e->capa <= e->size)
		e->capa *= 2;
	i = -1;
	e->envs = malloc(sizeof(t_env *) * e->capa);
	if (e->envs == NULL)
		return (1);
	if (envs_alloc((void **)e->envs, sizeof(t_env), 0, e->size) == 1)
		return (1);
	if (envs_element_alloc(e, 0, e->size) == 1)
		return (1);
	set_envs(e);
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
