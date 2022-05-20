/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:16:18 by sichoi            #+#    #+#             */
/*   Updated: 2022/05/20 15:34:36 by swi              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int main(int argc, char **argv, char **envp)
{
	t_envs			e;

	if (argc > 1)
		return (1);
	(void)argv;
	input_env(&e, envp);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	loop(&e);
	return (0);
}
