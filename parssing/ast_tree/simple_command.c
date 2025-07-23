/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 09:27:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/21 07:55:53 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	collect_args(char *cmd, t_node **list, char **args)
{
	t_node	*tmp;
	int	i;

	i = 0;
	args[i++] = ft_strdup(cmd);
	*list = (*list)->next;
	while ((*list) && ((*list)->type == WORD || (*list)->between_quoted))
	{
		args[i] = ft_strdup((*list)->content);
		i++;
		*list = (*list)->next;
	}
	tmp = *list;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp && is_redirection(tmp) && tmp->next)
		{
			tmp = tmp->next;
			tmp = tmp->next;
		}
		else if (tmp && (tmp->type == WORD || tmp->between_quoted))
		{
			args[i] = ft_strdup(tmp->content);
			i++;
			tmp = tmp->next;
		}
	}
	args[i] = 0;
}

t_tree  *command_without_redirection(t_node **list)
{
	t_tree	*node;
	char    *cmd;
	char    **args;
	int len;

	len = count_args(*list) + 1;
	cmd = ft_strdup((*list)->content);
	if (!(args = malloc(sizeof(char *) * len)))
		return (NULL);
	collect_args(cmd, list, args);
	node = malloc(sizeof(t_tree));
	if (!node)
	{
		free_str(args);
		return (NULL);
	}
	if (!(node->command = malloc(sizeof(t_command))))
	{
		free_str(args);
		free(node);
		return (NULL);
	}
	node->command->command = cmd;
	node->command->args = args;
	node->type = COMMAND_NODE;
	return (node);
}
