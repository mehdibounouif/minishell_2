/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/24 17:05:03 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init(t_tree *node)
{
	node->type = REDIRECT_NODE;
	node->redirect->in = 0;
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

	if (!(node = malloc(sizeof(t_files))))
		return (NULL);
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
			add_back1(&node->redirect->files, new_node((*list)->content, R_OUT));
			node->redirect->out_files[(*j)++] = ft_strdup((*list)->content);
			*list = (*list)->next;
		}
		else if (*list && (*list)->type == R_IN)
		{
			*list = (*list)->next;
			node->redirect->in_files[(*i)++] = ft_strdup((*list)->content);
			add_back1(&node->redirect->files, new_node((*list)->content, R_IN));
			*list = (*list)->next;
		}
		else
			*list = (*list)->next;
	}
//	print_list1(node->redirect->files);
}

void	collect_in_out_files2(t_node **list, t_tree *node, int *i, int *j)
{
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
			add_back1(&node->redirect->files, new_node((*list)->content, R_OUT));
			node->redirect->out_files[(*j)++] = ft_strdup((*list)->content);
			*list = (*list)->next;
		}
		else if (*list && (*list)->type == R_IN)
		{
			*list = (*list)->next;
			node->redirect->in_files[(*i)++] = ft_strdup((*list)->content);
			add_back1(&node->redirect->files, new_node((*list)->content, R_IN));
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

int	allocat_files_array(t_tree *node)
{
	int	infile_num;
	int	outfile_num;

	infile_num = node->redirect->in_count + 1;
	outfile_num = node->redirect->out_count + 1;
	node->redirect->in_files = malloc(sizeof(char *) * infile_num);
	if (!node->redirect->in_files)
		return (0);
	node->redirect->out_files = malloc(sizeof(char *) * outfile_num);
	if (!node->redirect->out_files)
		return (0); // FREE HERE;
	return (1);
}

void	skip_redirection(t_node **list)
{
		*list = (*list)->next;
		*list = (*list)->next;
}

int	allocat_node(t_tree **node)
{
	*node = malloc(sizeof(t_tree));
	if (!*node)
		return (0);
	(*node)->redirect = malloc(sizeof(t_redirection));
	if (!(*node)->redirect)
	{
		free_tree(node);
		free(node);
		return (0);
	}
	return (1);
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
	if (!allocat_node(&redirect_node))
		return (NULL);
	init(redirect_node);
	tmp = *list;
	collect_herdoc(redirect_node, tmp);
	if (!allocat_files_array(redirect_node))
		return (NULL); // Free here;
	redirect_node->redirect->files = NULL;
	collect_in_out_files2(list, redirect_node, &i, &j);
	if (*list && (*list)->type != PIPE)
		prev = command_without_redirection(list);
	if (prev == NULL)
		redirect_node->redirect->without_cmd = 1;
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
	if (!allocat_node(&redirect_node))
		return (NULL);
	init(redirect_node);
	tmp = *list;
	collect_herdoc(redirect_node, tmp);
	if (!allocat_files_array(redirect_node))
		return (NULL); // FREE HERE;
	redirect_node->redirect->files = NULL;
	collect_in_out_files(list, redirect_node, &i, &j);
	redirect_node->redirect->in_files[i] = NULL;
	redirect_node->redirect->out_files[j] = NULL;
	assign_last_file(redirect_node);
	redirect_node->redirect->prev = node;
	return (redirect_node);
}
