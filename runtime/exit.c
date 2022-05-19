/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 22:53:20 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/19 23:20:10 by sichoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void	wait_child(void)
{
	int	status;
	int	ret;

	status = -1;
	while (waitpid(-1, &status, 0) >= 0)
	{
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		else
		{
			ret = WTERMSIG(status);
			if (ret == SIGQUIT)
				write(STDERR_FILENO, "Quit: 3\n", 7);
			ret = 128 + ret;
		}
	}
	if (status != -1)
		g_exit_code = ret;
}

void	eof_exit(int col, int row)
{
	move_cursor(col, row);
	printf("exit\n");
	close(STDOUT_FILENO);
}