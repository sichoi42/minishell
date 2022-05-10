#include "minishell.h"
#include <stdlib.h>

void	free_envs(t_envs *e)
{
	int	i;

	i = -1;
	while (++i < e->size)
	{
		free(e->envs[i]->key);
		free(e->envs[i]->value);
		free(e->envs[i]);
		free(e->env[i]);
	}
	free(e->envs);
	free(e->env);
	free(e->pwd);
	free(e->old_pwd);
}

void	free_double_char(char **dptr)
{
	char **temp;

	temp = dptr;
	while (*temp != NULL)
	{
		free(*temp);
		temp += 1;
	}
	free(dptr);
}
