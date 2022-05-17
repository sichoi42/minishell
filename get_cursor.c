#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main()
{
	int		i;
	int 	ret;
	int		row;
	int		col;
	char	buf[1024];

	write(STDOUT_FILENO, "\033[6n", strlen("\033[6n"));
	ret = read(STDIN_FILENO, buf, 1024);
	buf[ret] = 0;
	i = 0;
	while (!isdigit(buf[i]))
		++i;
	row = atoi(&buf[i]) - 1;
	while (isdigit(buf[i]))
		++i;
	while (!isdigit(buf[i]))
		++i;
	col = atoi(&buf[i]) - 1;
	printf("%d, %d\n", row, col);
    return 0;
}