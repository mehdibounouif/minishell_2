/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:07:26 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 17:06:52 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_dollar(char *cmd, int i)
{
	if (cmd[i] == '$' && cmd[i + 1])
		return (1);
	return (0);
}

int	get_env_len(char *cmd, int i)
{
	int	j;

	j = 0;
	while (cmd[i] && (ft_isalnum(cmd[i]) || cmd[i] == '_'))
	{
		i++;
		j++;
	}
	return (j);
}

void	free_node(t_node *node)
{
	if (node)
	{
		if (node->content)
			free(node->content);
		free(node);
	}
}

void	replace_key(char *cmd, t_share *share, t_env *list)
{
	char	*value;
	char	*key;
	int		l;

	l = 0;
	key = get_env_key(cmd, share->i);
	value = ft_getenv(key, list);
	if (!value)
		value = ft_strdup("");
	while (value[l])
		share->expanded_cmd[(share->j)++] = value[l++];
	share->i += (ft_strlen(key) + 1);
}

t_node	*insert_sublist(t_node *start, t_node *new, t_node *next)
{
	t_node	*last;

	if (!new)
		return (start);
	if (start)
	{
		start->next = new;
		new->prev = start;
	}
	last = new;
	while (last->next)
		last = last->next;
	last->next = next;
	if (next)
		next->prev = last;
	if (start)
		return (start);
	else
		return (new);
}
