#include "minishell.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// dup2와 error check까지
int	dup_check(int fd_l, int fd_r)
{
	int	rtn;

	rtn = dup2(fd_l, fd_r);
	if (rtn == -1)
	{
		// 추후 struct 정해지면 파일이름 추가해서 에러 출력
		print_error("bash", strerror(errno), NULL, NULL);
		exit(1);
	}
	return (OK);
}

static int	red_here_doc(int *fd, char *limit)
{
	char	*str;

	while (1)
	{
		rl_replace_line("", 1);
		str = readline("> ");
		if (!str || ft_strcmp(str, limit) == 0)
			break ;
		write(*fd, str, ft_strlen(str));
		write(*fd, "\n", 1);
	}
	close(*fd);
	*fd = open(TEMP_FILE, O_RDWR);
	if (*fd == -1)
	{
		print_error("bash", TEMP_FILE, strerror(errno), NULL);
		return (WRONG_ACTION);
	}
	unlink(TEMP_FILE);
	return (dup_check(*fd, STDIN_FILENO));
}

static int	red_in(char *f_name)
{
	int fd;

	fd = open(f_name, O_RDWR);
	if (fd == -1)
		print_error("bash", f_name, strerror(errno), NULL);
	else
	{
		dup_check(fd, STDIN_FILENO);
		close(fd);
		return (OK);
	}
	return (WRONG_ACTION);
}

static int	red_out(char *f_name)
{
	int fd;

	fd = open(f_name, O_RDWR | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
		print_error("bash", f_name, strerror(errno), NULL);
	else
	{
		dup_check(fd, STDOUT_FILENO);
		close(fd);
		return (OK);
	}
	return (WRONG_ACTION);
}

static int	red_append(char *f_name)
{
	int fd;

	fd = open(f_name, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd == -1)
		print_error("bash", f_name, strerror(errno), NULL);
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

	if (t & T_RE_INPUT)
		return (red_in(f_name));
	else if (t & T_RE_APPEND_OUTPUT)
		return (red_append(f_name));
	else if (t & T_RE_OUTPUT)
		return (red_out(f_name));
	else if (t & T_RE_HEREDOC)
	{
		fd = open(TEMP_FILE, O_RDWR | O_CREAT | O_TRUNC, 0664);
		if (fd == -1)
		{
			print_error("bash", TEMP_FILE, strerror(errno), NULL);
			return (WRONG_ACTION);
		}
		return (red_here_doc(&fd, f_name));
	}
	return (WRONG_ACTION);
}

void	init_pipe(int *pipe_fd)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
}

void	close_pipe(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

int	make_pipe(int *p_fd)
{
	int	rtn;

	rtn = pipe(p_fd);
	if (rtn == -1)
	{
		print_error("bash", strerror(errno), NULL, NULL);
		exit(1);
	}
	dup_check(p_fd[1], STDOUT_FILENO);
	return (0);
}
