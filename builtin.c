#include "minishell.h"

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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

// ft_unset, ft_export
static int	check_key(char *key)
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

// ft_unset, ft_export
static int	error_key(char *oper, char *arg)
{
	int		len;
	char	*err_arg;

	len = ft_strlen(arg);
	err_arg = malloc_array(sizeof(char), len + 3);
	err_arg[0] = '`';
	ft_strcat(arg, err_arg);
	ft_strcat("\'", err_arg);
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
			rtn = error_key("unset", o->opers[i]);
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
			rtn = error_key("export", o->opers[i]);
		else
			insert_env(e, key, value);
	}
	if (i == 1)
		print_export(e);
	free(key);
	free(value);
	return (rtn);
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
