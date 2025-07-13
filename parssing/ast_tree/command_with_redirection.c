/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/13 16:49:18 by mbounoui         ###   ########.fr       */
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
	node->redirect->prev = node;
}

void	collect_in_out_files(t_node **list, t_tree *node, int *i, int *j)
{
	while (*list && is_redirection(*list))
	{
		if ((*list)->type == HEREDOC)
		{
			*list = (*list)->next;
			*list = (*list)->next;
		}
		else if (*list && ((*list)->type == R_OUT || (*list)->type == R_APPEND))
		{
			if ((*list)->type == R_OUT)
				node->redirect->out_type = R_OUT;
			else
				node->redirect->out_type = R_APPEND;
			*list = (*list)->next;
			node->redirect->out_files[(*j)++] = ft_strdup((*list)->content);
			*list = (*list)->next;
		}
		else if (*list && (*list)->type == R_IN)
		{
			*list = (*list)->next;
			node->redirect->in_files[(*i)++] = ft_strdup((*list)->content);
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
	node->redirect->out_file = last_out_file;
	if (node->redirect->in)
		node->redirect->in_file = last_in_file;
	else
		node->redirect->in_file = last_herdoc;
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

t_tree	*parss_redirection_in_start(t_node **list, t_env *env)
{
	t_tree	*redirect_node;
	t_tree	*prev;
	t_node *tmp;
	int	i;
	int	j;
	(void)env;

	i = 0;
	j = 0;
	prev = NULL;
	redirect_node = malloc(sizeof(t_tree));
	if (!redirect_node)
		return (NULL);
	redirect_node->redirect = malloc(sizeof(t_redirection));
	if (!redirect_node->redirect)
	{
		free(redirect_node);
		return (NULL);
	}
	init(redirect_node); tmp = *list; collect_herdoc(redirect_node, tmp);
	if (!allocat_files_array(redirect_node))
		return (NULL); // Free here;
	collect_in_out_files(list, redirect_node, &i, &j);
	if (*list && (*list)->type != PIPE)
	{
		prev = command_without_redirection(list);
	}
	if (*list && (*list)->type != PIPE)
	{
		collect_in_out_files(list, redirect_node, &i, &j);
	}
	redirect_node->redirect->in_files[i] = NULL;
	redirect_node->redirect->out_files[j] = NULL;
	assign_last_file(redirect_node);
	redirect_node->redirect->prev = prev;
	return (redirect_node);
}


t_tree  *parss_redirection(t_tree *node, t_node **list, t_env *env)
{
	t_tree  *redirect_node;
	t_node	*tmp;
	int	i;
	int	j;
	(void)env;

	i = 0;
	j = 0;
	redirect_node = malloc(sizeof(t_tree));
	if (!redirect_node)
	{
		free_tree(&node);
		return (NULL);
	}
	redirect_node->redirect = malloc(sizeof(t_redirection));
	if (!redirect_node->redirect)
	{
		free_tree(&node);
		free(redirect_node);
		return (NULL);
	}
	init(redirect_node);
	tmp = *list;
	collect_herdoc(redirect_node, tmp);
	if (!allocat_files_array(redirect_node))
		return (NULL); // FREE HERE;
	collect_in_out_files(list, redirect_node, &i, &j);
	redirect_node->redirect->in_files[i] = NULL;
	redirect_node->redirect->out_files[j] = NULL;
	assign_last_file(redirect_node);
	redirect_node->redirect->prev = node;
	return (redirect_node);
}
