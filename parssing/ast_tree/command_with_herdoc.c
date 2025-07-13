/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/13 14:53:14 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

char	*generate_file_name()
{
	char	*nb;
	static int	i;

	i++;
	nb = ft_itoa(i);
	return (ft_strjoin(HEREDOC_FILE, nb));
}

int	create_heredoc(t_redirection *list, t_env *env, int i)
{
	int	fd;
	char	*line;
	char	*file_name;

	file_name = generate_file_name();
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
	if (fd == -1)
		return (-1);
	list->heredoc_fds[i] = fd;
	line = readline(">");
	while (line && ft_strcmp(list->herdoc->delimeter, line))
	{
		if (!list->herdoc->quoted)
			line = expansion(line, env);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	return (0);
}

void	open_herdocs(t_tree *tree, t_env *env)
{
    if (!tree)
		return;
	int	i;
	if (tree->type == REDIRECT_NODE)
	{
		i = 0;
		tree->redirect->heredoc_fds = ft_calloc(20, sizeof(int));
		if (!tree->redirect->heredoc_fds)
			return ; // FREE HERE
		while (tree->redirect->herdoc)
		{
			create_heredoc(tree->redirect, env, i);
			tree->redirect->herdoc = tree->redirect->herdoc->next;
			i++;
		}
	}
	else if (tree->type == PIPE_NODE)
	{
		open_herdocs(tree->pipe->left, env);
		open_herdocs(tree->pipe->right, env);
	}
}

char	*get_last_herdoc(t_herdoc *list)
{
	if (!list)
		return (NULL);
	while (list->next)
	{
		list = list->next;
	}
	return (list->delimeter);
}
char	*get_last_file(char **list)
{
	if (!(*list) || !list)
		return (NULL);
	int	i;

	i = 0;
	while (list[i+1])
	{
		i++;
	}
	return (list[i]);
}
void	collect_herdoc(t_tree *node, t_node *list)
{
	t_herdoc *h_node;

	node->redirect->herdoc = NULL;
	while (list && list->type != PIPE)
	{
		if (list->type == R_IN)
			node->redirect->in_count++;
		if (list->type == R_OUT || list->type == R_APPEND)
			node->redirect->out_count++;
		if (list->type == HEREDOC && list->next)
		{
			h_node = malloc(sizeof(t_herdoc));
			if (!h_node)
				return ;
			h_node->quoted = 0;
			if (node->redirect->in)
			{
				node->redirect->hdc = 1;
				node->redirect->in = 0;
			}
			else
				node->redirect->hdc = 1;
			h_node->herdoc = list->content;
			list = list->next;
			h_node->delimeter = list->content;
			if (list->quoted)
				h_node->quoted = 1;
			h_node->next = NULL;
			add_back3(&node->redirect->herdoc, h_node);
		}
		if (list->type == R_IN && node->redirect->hdc)
		{
			node->redirect->hdc = 0;
			node->redirect->in = 1;
		}
		else if (list->type == R_IN && !node->redirect->hdc)
			node->redirect->in = 1;
		list = list->next;
	}
}
