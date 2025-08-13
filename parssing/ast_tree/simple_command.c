/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 09:27:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 07:01:31 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_real_separator(char *cmd, int i)
{
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

void	collect_args(char *cmd, t_node **list, char **args)
{
	t_node	*tmp;
	int		i;

	i = 0;
	args[i++] = ft_strdup(cmd);
	*list = (*list)->next;
	while ((*list) && (*list)->type == WORD)
	{
		args[i++] = ft_strdup((*list)->content);
		*list = (*list)->next;
	}
	tmp = *list;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp && is_redirection(tmp) && tmp->next)
			skip_redirection(&tmp);
		else if (tmp && tmp->type == WORD)
		{
			args[i++] = ft_strdup(tmp->content);
			tmp = tmp->next;
		}
	}
	args[i] = 0;
}

t_tree	*command_without_redirection(t_node **list)
{
	t_tree	*node;
	char	*cmd;
	char	**args;
	int		len;

	len = count_args(*list) + 1;
	cmd = ft_strdup((*list)->content);
	args = ft_malloc(sizeof(char *), len);
	collect_args(cmd, list, args);
	node = ft_malloc(sizeof(t_tree), 1);
	node->command = ft_malloc(sizeof(t_command), 1);
	node->command->command = cmd;
	node->command->args = args;
	node->type = COMMAND_NODE;
	return (node);
}
