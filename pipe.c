#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

void	count_words_alloc(char ***paths, char *org, char div[], int *count)
{
	int		i;
	int		w;

	w = 0;
	while (*org != '\0')
	{
		i = 0;
		while (div[i] != '\0')
		{
			if (*org == div[i++])
			{
				w = 0;
				break ;
			}
		}
		if (div[i] == '\0')
		{
			if (w == 0)
				++(*count);
			++w;
		}
		++org;
	}
	*paths = malloc_array(sizeof(char *), *count + 1);
}

void	count_word_alloc(char *org, char **words, char *div, int *max_len)
{
	int		i;
	int		len;

	len = 1;
	*max_len = 0;
	while (1)
	{
		i = -1;
		while (div[++i] != '\0')
			if (*org == div[i])
				break ;
		if (div[i] == '\0' && *org != '\0')
			++len;
		else
		{
			if (len != 1)
				*(words++) = malloc_array(sizeof(char), len);
			*max_len = ft_max(*max_len, len);
			len = 1;
		}
		if (*org++ == '\0')
			break ;
	}
	*words = NULL;
}

void	fill_array(char *org, char **words, char *div, int count)
{
	int		i;
	int		j;
	int		w;

	j = 0;
	w = 0;
	while (*org != '\0')
	{
		i = -1;
		while (div[++i] != '\0')
			if (*org == div[i])
				break ;
		if (div[i] == '\0')
			words[j][w++] = *org;
		else if (w != 0)
		{
			if (count - 1 == j)
				break ;
			words[j++][w] = '\0';
			w = 0;
		}
		++org;
	}
	words[j][w] = '\0';
}

char	*make_oper_path(char *oper_path, char *path, char *oper)
{
	char	*temp;

	temp = oper_path;
	while (*path != '\0')
		*(temp++) = *(path++);
	*(temp++) = '/';
	while (*oper != '\0')
		*(temp++) = *(oper++);
	*(temp) = '\0';
	return (oper_path);
}

void	find_path(char ***paths, int *max_path, t_envs *e)
{
	char	*path;
	int		count;

	//path = getenv("PATH");
	path = get_env_value(e, "PATH");
	count = 0;
	count_words_alloc(paths, path, ":", &count);
	count_word_alloc(path, *paths, ":", max_path);
	fill_array(path, *paths, ":", count);
}

static int	check_right_path(char **o_path, char **paths, char *oper)
{
	struct stat		s;
	int				i;
	int				rtn;
	unsigned char	exit_code;

	exit_code = 0;
	i = -1;
	while (1)
	{
		if (paths[++i] != NULL)
			*o_path = make_oper_path(*o_path, paths[i], oper);
		else
			ft_strcpy(oper, *o_path);
		rtn = stat(*o_path, &s);
		if (rtn != -1 && s.st_mode & S_IXUSR)
			return (OK);
		else if (!(s.st_mode & S_IXUSR))
			exit_code = 126;
		else
			if (exit_code != 126)
				exit_code = 127;
		if (paths[i] == NULL)
			break ;
	}
	return (exit_code);
}

char	*make_oper(char **opers, int max_path, char **paths)
{
	char		*oper_path;
	int			max_oper;

	max_oper = ft_strlen(opers[0]);
	oper_path = malloc_array(sizeof(char), max_path + max_oper + 1);
	if (check_right_path(&oper_path, paths, opers[0]) != OK)
	{
		free(oper_path);
		oper_path = NULL;
	}
	return (oper_path);
}

void exe_oper(t_oper *o, t_ast *node, t_envs *e)
{
	int pid;

	pid = fork();
	if (pid == 0)
	{
		if (node->root->pipe_cnt > 0)
			close_pipe(node->root->pipe_fd);
		if (built_in_check(o, e, node) == -1)
		{
			execve(o->oper_path, o->opers, e->env);
			print_error("minishell", o->opers[0], COMMAND_ERROR, NULL);
			exit(1);
		}
		exit(0);
	}
	else if (pid == -1)
		exit(1);
	else
		node->root->pid = pid;
	if (node->root->pipe_cnt > 0)
		dup_check(node->root->pipe_fd[0], STDIN_FILENO);
	else if (node->root->pipe_cnt == 0)
		close(STDIN_FILENO);
	if (node->root->pipe_cnt > 0)
		close_pipe(node->root->pipe_fd);
}
