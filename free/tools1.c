/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:07:52 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/22 10:26:15 by mbounoui         ###   ########.fr       */
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
		i++;
	}
	free(list);
}

void	free_list(t_node *list)
{
	t_node *tmp;

	if (!list)
		return ;
	tmp = list;
	while (list)
	{
		list = list->next;
		if (tmp->content)
			free(tmp->content);
		free(tmp);
		tmp = list;
	}
	list = NULL;
}

void	free_herdoc(t_herdoc *list)
{
	t_herdoc *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->herdoc)
			free(tmp->herdoc);
		if (tmp->delimeter)
			free(tmp->delimeter);
		if (tmp)
			free(tmp);
	}
}

void	free_files(t_files *files)
{
	t_files *tmp;

	while (files)
	{
		tmp = files;
		files = files->next;
		if (tmp->file)
			free(tmp->file);
		free(tmp->file);
	}
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

