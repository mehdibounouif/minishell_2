/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/12 10:23:26 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*parss_redirection_in_start(t_node **list, t_env *env)
{
	t_tree	*redirect_node;
	t_tree	*node;

	(void)env;
	redirect_node = malloc(sizeof(t_tree));
	if (!redirect_node)
		return (NULL);
	redirect_node->redirect = malloc(sizeof(t_redirection));
	if (!redirect_node->redirect)
	{
		free(redirect_node);
		return (NULL);
	}
	redirect_node->type = REDIRECT_NODE;
	redirect_node->redirect->in_redirect = (*list)->content;
//	redirect_node->redirect->redirection_type = (*list)->type;
	printf("%d\n", (*list)->type);
	*list = (*list)->next;
	redirect_node->redirect->in_file = (*list)->content;
	*list = (*list)->next;
	node = pars_one_side(list, env);
	redirect_node->redirect->prev = node;
	return (redirect_node);
}

void	init(t_tree *node)
{
	node->type = REDIRECT_NODE;
	node->redirect->in = 0;
	node->redirect->count = 0;
	node->redirect->out = 0;
	node->redirect->hdc = 0;
	node->redirect->prev = node;

}

t_tree  *parss_redirection(t_tree *node, t_node **list, t_env *env)
{
	t_tree  *redirect_node;
	t_node	*tmp;
	int	i;
	(void)env;

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
	redirect_node->redirect->in_files = malloc(sizeof(char *) * redirect_node->redirect->count + 1);
	i = 0;
	while (*list && (*list)->type != PIPE)
	{
		if ((*list)->type == HEREDOC)
		{
			redirect_node->redirect->in_redirect = (*list)->content;
			if (redirect_node->redirect->hdc)
				redirect_node->redirect->in_file = get_last_herdoc(redirect_node->redirect->herdoc);
			else
				redirect_node->redirect->in_file = (*list)->content;
			*list = (*list)->next;
			*list = (*list)->next;
		}
		else if (*list && ((*list)->type == R_OUT || (*list)->type == R_APPEND))
		{
			redirect_node->redirect->out++; redirect_node->redirect->out_redirect = (*list)->content;
			*list = (*list)->next;
			redirect_node->redirect->out_file = (*list)->content;
			*list = (*list)->next;
		}
		else if (*list && (*list)->type == R_IN)
		{
			redirect_node->redirect->in_redirect = (*list)->content;
			*list = (*list)->next;
			redirect_node->redirect->in_files[i++] = ft_strdup((*list)->content);
			*list = (*list)->next;
		}
	}
	redirect_node->redirect->in_files[i] = NULL;
	redirect_node->redirect->prev = node;
	return (redirect_node);
}
