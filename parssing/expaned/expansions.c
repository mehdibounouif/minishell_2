/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:23:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 02:01:01 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_cmd(char *cmd, t_share *share, t_env *env, int b_q)
{
	while (cmd[share->i])
	{
		while (is_dollar(cmd, share->i) && b_q != 1
			&& (ft_isalpha(cmd[share->i + 1]) || cmd[share->i + 1] == '_'))
			replace_key(cmd, share, env);
		if (!cmd[share->i])
			break ;
		if (is_dollar(cmd, share->i) && cmd[share->i + 1] == '?' && b_q != 1)
			expand_exit_status(share);
		else
			share->expanded_cmd[share->j++] = cmd[share->i++];
	}
}

char	*expansion(char *cmd, t_env *env, int b_q)
{
	int		full_len;
	t_share	*share;

	share = ft_malloc(sizeof(t_share), 1);
	share->i = 0;
	share->j = 0;
	share->h = 0;
	full_len = get_full_len(cmd, env);
	if (full_len == 0)
		return (ft_strdup(""));
	share->expanded_cmd = ft_malloc(sizeof(char), full_len + 1);
	expand_cmd(cmd, share, env, b_q);
	share->expanded_cmd[share->j] = '\0';
	return (share->expanded_cmd);
}

t_node	*create_list(char *content, t_node *tmp)
{
	char	**list;
	t_node	*node;
	t_node	*head;
	int		i;
	int		len;

	i = 0;
	head = NULL;
	list = ft_split(content, ' ');
	len = ft_arraylen(list);
	while (list[i])
	{
		node = create_node2(list[i], tmp->b_space, tmp->type,
				tmp->between_quoted);
		i++;
		if (i < len || (i == len && content[ft_strlen(content) - 1] == ' '))
			node->b_space = 0;
		add_back(&head, node);
	}
	return (head);
}

static t_node	*expand_list(t_node *prev, t_node *tmp, t_env *env)
{
	char	*content;

	content = expansion(tmp->content, env, tmp->between_quoted);
	if (tmp->between_quoted || content[0] == '\0' || only_space(content))
		return (create_node(content, tmp->b_space, tmp->between_quoted));
	if (content[0] == ' ' && prev)
		prev->b_space = 0;
	return (create_list(content, tmp));
}

void	expand(t_node **list, t_env *env)
{	
	t_expand	*s;

	s = ft_malloc(sizeof(t_expand), 1);
	s->tmp = *list;
	while (s->tmp)
	{
		if (s->tmp->type == HEREDOC)
		{
			s->tmp = s->tmp->next;
			if (s->tmp)
				s->tmp = s->tmp->next;
			continue ;
		}
		s->start = s->tmp->prev;
		s->next = s->tmp->next;
		s->to_free = s->tmp;
		s->sub_list = expand_list(s->tmp->prev, s->tmp, env);
		s->new_list = insert_sublist(s->start, s->sub_list, s->next);
		if (!s->start)
			*list = s->new_list;
		s->tmp = s->next;
		free_node(s->to_free);
	}
}
