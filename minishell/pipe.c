#include "minishell.h"

void	*malloc_array(int size, int len)
{
	void	*temp;

	temp = malloc(size * len);
	if (temp == NULL)
		exit(1);
	return (temp);
}

int	ft_max(int l, int r)
{
	if (l > r)
		return (l);
	return (r);
}

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

char	*make_oper(char ***opers, int max_path, char **paths, char *argv)
{
	char		*oper_path;
	int			max_oper;
	int			i;
	int			count;
	int			rtn;
	struct stat	s;

	count = 0;
	count_words_alloc(opers, argv, " \t", &count);
	count_word_alloc(argv, *opers, " \t", &max_oper);
	fill_array(argv, *opers, " \t", count);
	oper_path = malloc_array(sizeof(char), max_path + max_oper);
	i = -1;
	while (paths[++i] != NULL)
	{
		oper_path = make_oper_path(oper_path, paths[i], (*opers)[0]);
		rtn = stat(oper_path, &s);
		if (rtn != -1 && s.st_mode & S_IXUSR)
			break ;
	}
	return (oper_path);
}
