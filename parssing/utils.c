/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:01:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/24 17:04:44 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_empty(char *s)
{
	if (ft_strlen(s))
		return (1);
	return (0);
}

int	is_redirection(t_node *node)
{
	if (node && (node->type == R_OUT || node->type == R_IN
			|| node->type == R_APPEND || node->type == HEREDOC))
		return (1);
	return (0);
}

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	is_separator(char *line, int i)
{
	if ((line[i] && line[i + 1] && (!ft_strncmp(&line[i], ">>", 2)
				|| !ft_strncmp(&line[i], "<<", 2))) || (line[i]
			&& (!ft_strncmp(&line[i], "|", 1) || !ft_strncmp(&line[i], ">", 1)
				|| !ft_strncmp(&line[i], "<", 1))))
		return (1);
	return (0);
}

int	is_real_separator(char *cmd, int i)
{
	// if (i > 0 && cmd[i - 1] == '\\' &&  is_separator(&cmd[i], i))
	//  return (0);
	if (is_separator(&cmd[i], i) && check_quotes(cmd, i) == 0)
		return (1);
	else
		return (0);
}

int	count_args(t_node *list)
{
	int	i;

	i = 0;
	while (list && list->type != PIPE)
	{
		if (list && list->type == WORD)
		{
			i++;
			list = list->next;
		}
		else if (list && is_redirection(list) && list->next)
		{
			list = list->next;
			list = list->next;
		}
	}
	return (i);
}

t_node	*create_node(const char *content, int b_space, int b_q)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->content = ft_strdup1(content);
	new->b_space = b_space;
	new->between_quoted = b_q;
	new->contain_quoted = 1;
	new->type = 2;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_node	*create_node2(const char *content, int b_space, int type, int b_q)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->content = ft_strdup1(content);
	new->b_space = b_space;
	new->between_quoted = b_q;
	new->contain_quoted = 1;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_back3(t_herdoc **list, t_herdoc *node)
{
	t_herdoc	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back2(t_env **list, t_env *node)
{
	t_env	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back1(t_files **list, t_files *node)
{
	t_files	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back(t_node **list, t_node *node)
{
	t_node	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
}
