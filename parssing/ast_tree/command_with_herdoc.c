/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/12 10:46:47 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
int	is_quoted(char *cmd, int len)
{
	return ((cmd[0] == '\'' && cmd[len] == '\'') || (cmd[0] == '\"' || cmd[len] == '\''));
}
*/

char	*generate_file_name()
{
	char	*nb;
	static int	i;

	i++;
	nb = ft_itoa(i);
	return (nb);
}

int	create_heredoc(t_herdoc *list, t_env *env)
{
	int	fd;
	static	int	old_fd;
	char	*line;
	char	*nb;
	char	*file_name;

	nb = generate_file_name();
	if (old_fd)
		close(old_fd);
	printf("old fd = %d\n", old_fd);
	file_name = ft_strjoin(HEREDOC_FILE, nb);
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
	if (fd == -1)
		return (-1);
	old_fd = fd;
	line = readline(">");
	while (line && ft_strcmp(list->delimeter, line))
	{
		if (!list->quoted)
		{
			line = expansion(line, env);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	//herdoc_node->redirect->her_file = file_name;
	printf("fd = %d\n", fd);
	return (0);
}

char	*get_last_herdoc(t_herdoc *list)
{
	while (list->next)
	{
		list = list->next;
	}
	return (list->delimeter);
}

void	collect_herdoc(t_tree *node, t_node *list)
{
	t_herdoc *h_node;

	node->redirect->herdoc = NULL;
	while (list && list->type != PIPE)
	{
		if (list->type == R_IN)
			node->redirect->count++;
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

t_tree	*parss_herdoc(t_tree *node, t_node *list, t_env *env)
{
	t_tree	*herdoc_node;

	herdoc_node = malloc(sizeof(t_tree));
	if (!herdoc_node)
		return (NULL);
	herdoc_node->redirect = malloc(sizeof(t_redirection));
	if (!herdoc_node->redirect)
	{
		free(herdoc_node);
		return (NULL);
	}
	//herdoc_node->redirect->her_redirect = list->content;
	while(list)
	{
		if (list->type == HEREDOC && list->next)
		{
			list = list->next;
//			if (create_heredoc(list, env))
				return (NULL);
		}
		list = list->next;
	}
	herdoc_node->type = REDIRECT_NODE;
	herdoc_node->redirect->prev = node;
	return (herdoc_node);
}

void	fill_herdoc(t_herdoc *list, t_env *env)
{
	while (list)
	{
		list = list->next;
	}
}

void	open_herdocs(t_tree *tree, t_env *env)
{
    if (!tree)
		return;

	if (tree->type == REDIRECT_NODE)
		fill_herdoc(tree->redirect->herdoc, env);
	else if (tree->type == PIPE_NODE)
	{
		open_herdocs(tree->pipe->left, env);
		open_herdocs(tree->pipe->right, env);
	}
	printf("\n");
}
