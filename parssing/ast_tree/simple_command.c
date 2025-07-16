/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 09:27:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/13 16:45:35 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree  *command_without_redirection(t_node **list)
{
	t_tree	*node;
	int	i;
	char    *cmd;
	char    **args;
	int len;

	i = 0;
	len = count_args(*list);
	cmd = (*list)->content;
	args = malloc(sizeof(char *) * len + 1);
	args[i++] = cmd;
	*list = (*list)->next;
	while ((*list) && ((*list)->type == WORD || (*list)->between_quoted))
	{
		args[i] = (*list)->content;
		i++;
		*list = (*list)->next;
	}
	args[i] = 0;
	node = malloc(sizeof(t_tree));
	if (!node)
	{
		free_str(args);
		return (NULL);
	}
	node->command = malloc(sizeof(t_command));
	if (!node->command)
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
