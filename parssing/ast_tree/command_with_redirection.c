/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 13:16:48 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct s_share4
{
	t_tree	*redirect_node;
	t_tree	*prev;
	t_node *tmp;
	int	i;
	int	j;

}	t_share4;

void	init(t_tree *node)
{
	node->type = REDIRECT_NODE;
	node->redirect->in = 0;
	node->redirect->out_type = 0;
	node->redirect->in_count = 0;
	node->redirect->out_count = 0;
	node->redirect->hdc = 0;
	node->redirect->files = NULL;
	node->redirect->herdoc = NULL;
}

t_files	*new_node(char *content, int type)
{
	t_files *node;

	node = ft_malloc(sizeof(t_files) , 1);
	node->file = ft_strdup(content);
	node->type = type;
	node->next = NULL;
	return (node);
}

void	assing_io(t_node **list, t_share4 *share)
{
	t_files	*file;

	if (*list && ((*list)->type == R_OUT || (*list)->type == R_APPEND))
	{
		share->redirect_node->redirect->out_type = (*list)->type;
		*list = (*list)->next;
		file = new_node((*list)->content, (*list)->type);
		add_back1(&share->redirect_node->redirect->files,file);
		share->redirect_node->redirect->out_files[share->j++] = ft_strdup((*list)->content);
		*list = (*list)->next;
	}
	else if (*list && (*list)->type == R_IN)
	{
		*list = (*list)->next;
		share->redirect_node->redirect->in_files[share->i++] = ft_strdup((*list)->content);
		file = new_node((*list)->content, R_IN);
		add_back1(&share->redirect_node->redirect->files,file);
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
		if (*list && (*list)->type != R_IN &&
			(*list)->type != R_APPEND
			&& (*list)->type != R_OUT
			&& (*list)->type != HEREDOC
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

void	allocat_files_array(t_tree *node)
{
	int	infile_num;
	int	outfile_num;

	infile_num = node->redirect->in_count + 1;
	outfile_num = node->redirect->out_count + 1;
	node->redirect->in_files = ft_malloc(sizeof(char *) , infile_num);
	node->redirect->out_files = ft_malloc(sizeof(char *) , outfile_num);
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
	t_share4 *share;

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

t_tree  *parss_redirection(t_tree *node, t_node **list)
{
	t_share4 *share;
	t_node	*tmp;

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
