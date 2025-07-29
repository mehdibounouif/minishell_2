/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/28 10:43:58 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init(t_tree *node)
{
	node->type = REDIRECT_NODE;
	node->redirect->in = 0;
	node->redirect->out_type = 0;
	node->redirect->in_count = 0;
	node->redirect->out_count = 0;
	node->redirect->hdc = 0;
	node->redirect->last_fd = -1;
	node->redirect->files = NULL;
	node->redirect->without_cmd = 0;
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

//void	add_back1(t_files **list, t_files *node)
//{
//	t_files *tmp;
//	if (!list || !node)
//		return ;
//	if (!*list)
//		*list = node;
//	else
//	{
//		tmp = *list;
//		while (tmp->next)
//			tmp = tmp->next;
//		tmp->next = node;
//	}
//}

void	collect_in_out_files(t_node **list, t_tree *node, int *i, int *j)
{
	t_files	*file;

	while (*list && (*list)->type != PIPE)
	{
		if ((*list)->type == HEREDOC)
			skip_redirection(list);
		else if (*list && ((*list)->type == R_OUT || (*list)->type == R_APPEND))
		{
			if ((*list)->type == R_OUT)
				node->redirect->out_type = R_OUT;
			else
				node->redirect->out_type = R_APPEND;
			*list = (*list)->next;
			file = new_node((*list)->content, R_OUT);
			add_back1(&node->redirect->files,file);
			node->redirect->out_files[(*j)++] = ft_strdup((*list)->content);
			*list = (*list)->next;
		}
		else if (*list && (*list)->type == R_IN)
		{
			*list = (*list)->next;
			node->redirect->in_files[(*i)++] = ft_strdup((*list)->content);
			file = new_node((*list)->content, R_IN);
			add_back1(&node->redirect->files,file);
			*list = (*list)->next;
		}
		else
			*list = (*list)->next;
	}
}

void	collect_in_out_files2(t_node **list, t_tree *node, int *i, int *j)
{
	t_files *file;

	while (*list && is_redirection(*list))
	{
		if ((*list)->type == HEREDOC)
			skip_redirection(list);
		else if (*list && ((*list)->type == R_OUT || (*list)->type == R_APPEND))
		{
			if ((*list)->type == R_OUT)
				node->redirect->out_type = R_OUT;
			else
				node->redirect->out_type = R_APPEND;
			*list = (*list)->next;
			file = new_node((*list)->content, R_OUT);
			add_back1(&node->redirect->files, file);
			node->redirect->out_files[(*j)++] = ft_strdup((*list)->content);
			*list = (*list)->next;
		}
		else if (*list && (*list)->type == R_IN)
		{
			*list = (*list)->next;
			node->redirect->in_files[(*i)++] = ft_strdup((*list)->content);
			file = new_node((*list)->content, R_IN);
			add_back1(&node->redirect->files, file);
			*list = (*list)->next;
		}
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

void	allocat_node(t_tree **node)
{
	*node = ft_malloc(sizeof(t_tree), 1);
	(*node)->redirect = ft_malloc(sizeof(t_redirection), 1);
}

t_tree	*parss_redirection_in_start(t_node **list)
{
	t_tree	*redirect_node;
	t_tree	*prev;
	t_node *tmp;
	int	(i), (j);

	i = 0;
	j = 0;
	prev = NULL;
	redirect_node = NULL;
	allocat_node(&redirect_node);
	init(redirect_node);
	tmp = *list;
	collect_herdoc(redirect_node, tmp);
	allocat_files_array(redirect_node);
	redirect_node->redirect->files = NULL;
	collect_in_out_files2(list, redirect_node, &i, &j);
	if (!(*list) || (*list)->type == PIPE)
		redirect_node->redirect->without_cmd = 1;
	else
		prev = command_without_redirection(list);
	if (*list && (*list)->type != PIPE)
		collect_in_out_files(list, redirect_node, &i, &j);
	redirect_node->redirect->in_files[i] = NULL;
	redirect_node->redirect->out_files[j] = NULL;
	assign_last_file(redirect_node);
	redirect_node->redirect->prev = prev;
	return (redirect_node);
}

t_tree  *parss_redirection(t_tree *node, t_node **list)
{
	t_tree  *redirect_node;
	t_node	*tmp;
	int	i;
	int	j;

	i = 0;
	j = 0;
	redirect_node = NULL;
	allocat_node(&redirect_node);
	init(redirect_node);
	tmp = *list;
	collect_herdoc(redirect_node, tmp);
	allocat_files_array(redirect_node);
	redirect_node->redirect->files = NULL;
	collect_in_out_files(list, redirect_node, &i, &j);
	redirect_node->redirect->in_files[i] = NULL;
	redirect_node->redirect->out_files[j] = NULL;
	assign_last_file(redirect_node);
	redirect_node->redirect->prev = node;
	return (redirect_node);
}
