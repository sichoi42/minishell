#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stddef.h>

// void	handler(int signum)
// {

// }

int main(void)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve("/bin/abc", NULL, NULL);
	}
	else if (pid == -1)
		return (1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		printf("parent get exit code: %d, signal: %d\n", WEXITSTATUS(status), WTERMSIG(status));
	else if (WIFSIGNALED(status))
	printf("parent get exit code: %d, signal: %d\n", WEXITSTATUS(status), WTERMSIG(status));
}
