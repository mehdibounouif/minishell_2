/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:07:52 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/01 22:04:32 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_str(char **list)
{
	int i;

	if (!list || !*list)
		return ;
	i = 0;
	while (list[i])
	{
		free(list[i]);
		list[i] = NULL;
		i++;
	}
	free(list);
}

void free_env(t_env *env)
{
	t_env *temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void	free_list(t_node **list)
{
	t_node *tmp;
	tmp = *list;
	while (*list)
	{
		(*list) = (*list)->next;
		free(tmp->content);
		free(tmp);
		tmp = (*list);
	}
}












