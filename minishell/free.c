#include "minishell.h"
#include <stdlib.h>

void	free_envs(t_envs *e, t_flevel level)
{
	int	i;

	i = -1;
	if (level == BEF_KEY_SET)
	{
		while (++i < e->size)
			free(e->envs[i]);
		free(e->envs);
	}
	else if (level == AFT_KEY_SET)
	{
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

