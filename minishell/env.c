#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

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

static void	find_env_r(t_envs *e, char *key, int last)
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

static void	find_env_l(t_envs *e, char *key, int last)
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

static void	insert_main_env(t_envs *e, int i, char *key, char *value)
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

static void	change_exist_env(t_envs *e, int i, char *value)
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

static void	delete_main_env(t_envs *e, int i, char *key)
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
