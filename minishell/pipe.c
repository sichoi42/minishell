#include "minishell.h"
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

void	find_path(char ***paths, int *max_path)
{
	char	*path;
	int		count;

	path = getenv("PATH");
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

char	*make_oper(char ***opers, int max_path, char **paths, char *argv)
{
	char		*oper_path;
	int			max_oper;
	int			count;
	int			rtn;
	struct stat	s;

	count = 0;
	count_words_alloc(opers, argv, " \t", &count);
	count_word_alloc(argv, *opers, " \t", &max_oper);
	fill_array(argv, *opers, " \t", count);
	oper_path = malloc_array(sizeof(char), max_path + max_oper);
	if (check_right_path(&oper_path, paths, (*opers)[0]) != OK)
	{
		free(oper_path);
		oper_path = NULL;
	}
	/*
	{
		rtn = stat((*opers)[0], &s);
		if (rtn != -1 && s.st_mode & S_IXUSR)
			ft_strcpy((*opers)[0], oper_path);
		else
		{
			free(oper_path);
			oper_path = NULL;
		}
	}
	*/
	return (oper_path);
}

void	exe_oper(t_oper *o, int *pipe_fd, char *envp[])
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		// pipe가 있으면
		//dup_check(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		// pipe가 있으면
		// 빌트인 체크
		// 빌트인이면 빌트인 실행
		// 아니면
		execve(o->oper_path, o->opers, envp);
		print_error("bash", o->opers[0], strerror(errno), NULL);
		exit(1);
	}
	else if (pid == -1)
		exit(1);
	// pipe가 있으면
	//dup_check(pipe_fd[0], STDIN_FILENO);
	//마지막일 경우
	//close(STDIN_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}