/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:27:42 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/29 14:00:59 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

void	close_fds(int *list)
{
	int	i;

	if (!list[0])
		return ;
	i = 0;
	while (list[i] != 0)
	{
		close(list[i]);
		i++;
	}
}

void	free_redirect_node(t_redirection *node)
{
	if (!node)
		return ;
	if (node->in_file)
		free(node->in_file);
	if (node->out_file)
		free(node->out_file);
	free_str(node->in_files);
	free_str(node->out_files);
	free_str(node->heredocs);
	free_files(node->files);
	free_herdoc(node->herdoc);
//	if (node->fds_list)
//		close_fds(node->fds_list);
	free(node);
}

void	free_command_node(t_command *node)
{
	free_str(node->args);
	if (node->command)
		free(node->command);
}
