#include "minishell.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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

void	*malloc_array(int size, int len)
{
	void	*temp;

	temp = malloc(size * len);
	if (temp == NULL)
	{
		print_error("bash", strerror(errno), NULL, NULL);
		exit(1);
	}
	return (temp);
}

int	ft_max(int l, int r)
{
	if (l > r)
		return (l);
	return (r);
}

static int	ft_isspace(int c)
{
	if ((c >= 0x09 && c <= 0x0d) || c == ' ')
		return (1);
	else
		return (0);
}

static void	check_minus(const char *str, size_t *index, int *is_minus)
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
