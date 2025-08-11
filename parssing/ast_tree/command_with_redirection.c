/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 01:33:37 by moraouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	allocat_files_array(t_tree *node)
{
	int	infile_num;
	int	outfile_num;

	infile_num = node->redirect->in_count + 1;
	outfile_num = node->redirect->out_count + 1;
	node->redirect->in_files = ft_malloc(sizeof(char *), infile_num);
	node->redirect->out_files = ft_malloc(sizeof(char *), outfile_num);
}

void	skip_redirection(t_node **list)
{
	*list = (*list)->next;
	*list = (*list)->next;
}

void	allocat_node(t_share4 *share)
{
	share->redirect_node = ft_malloc(sizeof(t_tree), 1);
	share->redirect_node->redirect = ft_malloc(sizeof(t_redirection), 1);
}

t_tree	*parss_redirection_in_start(t_node **list)
{
	t_share4	*share;

	share = ft_malloc(sizeof(t_share4), 1);
	share = ft_memset(share, 0, sizeof(t_share4));
	allocat_node(share);
	init(share->redirect_node);
	share->tmp = *list;
	collect_herdoc(share->redirect_node, share->tmp);
	allocat_files_array(share->redirect_node);
	share->redirect_node->redirect->files = NULL;
	collect_in_out_files2(list, share);
	if (*list && (*list)->type != PIPE && !is_redirection(*list))
		share->prev = command_without_redirection(list);
	if (*list && (*list)->type != PIPE)
		collect_in_out_files(list, share);
	share->redirect_node->redirect->in_files[share->i] = NULL;
	share->redirect_node->redirect->out_files[share->j] = NULL;
	assign_last_file(share->redirect_node);
	share->redirect_node->redirect->prev = share->prev;
	return (share->redirect_node);
}

t_tree	*parss_redirection(t_tree *node, t_node **list)
{
	t_share4	*share;
	t_node		*tmp;

	share = ft_malloc(sizeof(t_share4), 1);
	share = ft_memset(share, 0, sizeof(t_share4));
	allocat_node(share);
	init(share->redirect_node);
	tmp = *list;
	collect_herdoc(share->redirect_node, tmp);
	allocat_files_array(share->redirect_node);
	share->redirect_node->redirect->files = NULL;
	collect_in_out_files(list, share);
	share->redirect_node->redirect->in_files[share->i] = NULL;
	share->redirect_node->redirect->out_files[share->j] = NULL;
	assign_last_file(share->redirect_node);
	share->redirect_node->redirect->prev = node;
	return (share->redirect_node);
}
