/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:48 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/09 15:45:49 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	unset_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->key && ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
			{
				*env = current->next; 
				return ;
			}
			//free(current->key);
			//free(current->value);
			//free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}
int	unset_command(t_env **env, char **args)
{
	int i;

	if (!args[1])
		return (EXIT_SUCCESS);
	i = 1;

	while (args[i])
	{
		unset_env_var(env, args[i]);
		i++;
	}
	return (global(0));
}
