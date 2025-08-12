/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 01:49:15 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/12 12:27:33 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_files	*new_node(char *content, int type)
{
	t_files	*node;

	node = ft_malloc(sizeof(t_files), 1);
	node->file = ft_strdup(content);
	node->type = type;
	node->next = NULL;
	return (node);
}

void	assing_io(t_node **list, t_share4 *share)
{
	t_files	*file;
	char	*content;

	if (*list && ((*list)->type == R_OUT || (*list)->type == R_APPEND))
	{
		share->redirect_node->redirect->out_type = (*list)->type;
		*list = (*list)->next;
		file = new_node((*list)->content, (*list)->prev->type);
		add_back1(&share->redirect_node->redirect->files, file);
		content = ft_strdup((*list)->content);
		share->redirect_node->redirect->out_files[share->j++] = content;
		*list = (*list)->next;
	}
	else if (*list && (*list)->type == R_IN)
	{
		*list = (*list)->next;
		content = ft_strdup((*list)->content);
		share->redirect_node->redirect->in_files[share->i++] = content;
		file = new_node((*list)->content, R_IN);
		add_back1(&share->redirect_node->redirect->files, file);
		*list = (*list)->next;
	}
}

void	collect_in_out_files(t_node **list, t_share4 *share)
{
	while (*list && (*list)->type != PIPE)
	{
		if ((*list)->type == HEREDOC)
			skip_redirection(list);
		assing_io(list, share);
		if (*list && (*list)->type != R_IN && (*list)->type != R_APPEND
			&& (*list)->type != R_OUT && (*list)->type != HEREDOC
			&& (*list)->type != PIPE)
			*list = (*list)->next;
	}
}

void	collect_in_out_files2(t_node **list, t_share4 *share)
{
	while (*list && is_redirection(*list))
	{
		if ((*list)->type == HEREDOC)
			skip_redirection(list);
		assing_io(list, share);
	}
}

void	assign_last_file(t_tree *node)
{
	char	*last_in_file;
	char	*last_out_file;
	char	*last_herdoc;

	last_in_file = get_last_file(node->redirect->in_files);
	last_out_file = get_last_file(node->redirect->out_files);
	last_herdoc = get_last_herdoc(node->redirect->herdoc);
	node->redirect->out_file = ft_strdup(last_out_file);
	if (node->redirect->in)
		node->redirect->in_file = ft_strdup(last_in_file);
	else
		node->redirect->in_file = ft_strdup(last_herdoc);
}
