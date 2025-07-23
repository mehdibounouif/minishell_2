/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:16:34 by moraouf           #+#    #+#             */
/*   Updated: 2025/06/26 16:53:18 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_node(t_env **lst, t_env *node)
{
	t_env	*temp;

	if (lst)
	{
		if (*lst)
		{
			temp = *lst;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = node;
		}
		else
			*lst = node;
	}
}

void	ft_swap(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;

	if (!a || !b)
		return ;
	tmp_key = a->key;
	a->key = b->key;
	b->key = tmp_key;
	tmp_value = a->value;
	a->value = b->value;
	b->value = tmp_value;
}

void	sorted_env(t_env *env)
{
	t_env	*tmp;
	int		sorted;

	if (!env)
		return ;
	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		tmp = env;
		while (tmp && tmp->next)
		{
			if (strcmp(tmp->key, tmp->next->key) > 0)
			{
				ft_swap(tmp, tmp->next);
				sorted = 0;
			}
			tmp = tmp->next;
		}
	}
}

t_env	*get_env_var(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
