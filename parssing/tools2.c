/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:16:04 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 02:22:11 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_env(t_env **envp, char **env)
{
	int		i;
	char	**key_value;
	int		key_len;
	int		env_len;
	t_env	*env_node;

	i = 0;
	while (env[i])
	{
		env_node = malloc(sizeof(t_env));
		key_value = ft_split1(env[i], '=');
		env_node->key = ft_strdup1(key_value[0]);
		key_len = ft_strlen(env_node->key);
		env_len = ft_strlen(env[i]);
		env_node->value = ft_substr1(env[i], key_len + 1, (env_len - key_len));
		env_node->next = NULL;
		add_back2(envp, env_node);
		free_str(key_value);
		i++;
	}
	return (1);
}

void	without_quotes(t_node **list)
{
	t_node	*tmp;
	char	*content;

	tmp = *list;
	while (tmp)
	{
		content = tmp->content;
		tmp->content = remove_quotes(tmp->content);
		free(content);
		tmp = tmp->next;
	}
}

void	empty(t_share1 *s, t_node **list, int *end)
{
	if (s->flag)
		*end = 1;
	s->next = s->tmp->next;
	s->prev = s->tmp->prev;
	if (s->prev)
		s->prev->next = s->next;
	else
		*list = s->next;
	if (s->next)
		s->next->prev = s->prev;
	free(s->tmp->content);
	free(s->tmp);
	s->tmp = s->next;
}

void	remove_empty(t_node **list, int *end)
{
	t_share1	*s;

	s = ft_malloc(sizeof(t_share1), 1);
	s->tmp = *list;
	s->flag = 0;
	while (s->tmp)
	{
		if (s->tmp->content
			&& s->tmp->content[0] == '\0' && !s->tmp->between_quoted)
			empty(s, list, end);
		else
		{
			if (s->tmp->type != PIPE)
				s->flag = 1;
			s->tmp = s->tmp->next;
		}
	}
}

void	remove_pipe_in_end(t_node **list, int flag)
{
	t_node	*tmp;
	t_node	*current;

	if (!list || !*list)
		return ;
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	while (tmp && tmp->type == PIPE)
	{
		if (flag)
			if (tmp->type == PIPE && tmp->prev && tmp->prev->type != PIPE)
				break ;
		current = tmp;
		tmp = tmp->prev;
		if (tmp)
			tmp->next = current->next;
		else
			*list = NULL;
		free(current->content);
		free(current);
	}
}
