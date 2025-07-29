/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/29 09:44:01 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/07/24 13:25:42 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

char	*generate_file_name()
{
	char	*nb;
	char  *name;
	static int	i;

	i++;
	nb = ft_itoa(i);
	name = ft_strjoin(HEREDOC_FILE, nb);
	return (name);
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

int	get_lastfd(int *list)
{
	int	i;

	i = 0;
	while (list[i] != -1)
		i++;
	return (list[i-1]);
}

int	check_line(char *file_name, char *line, int fd)
{
	if(!line)
	{
		free(file_name);
		close(fd);
		return (0);
	}
	return (1);
}

int	create_heredoc(t_redirection *list, t_env *env, int i)
{
	int	fd;
	char	*line;
	char	*file_name;
	char	*expand_line;

	file_name = generate_file_name();
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
	if (fd == -1)
	{
		free(file_name);
		printf("open heredoc file failed!\n");
		return (0);
	}
	list->heredoc_fds[i] = fd;
	list->heredocs[i] = ft_strdup(file_name);
	free(file_name);
	close(fd);
	line = readline(">");
	if (!check_line(file_name, line, fd))
		return (0);
	while (line && ft_strcmp(list->herdoc->delimeter, line))
	{
		if (!list->herdoc->quoted)
		{
			expand_line = expansion(line, env);
			write(fd, expand_line, ft_strlen(expand_line));
			write(fd, "\n", 1);
		}
		else 
		{
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		line = readline(">");
		if (!check_line(file_name, line, fd))
			return (0);
	}
	return (1);
}

int	open_herdocs(t_tree *tree, t_env *env)
{
	if (!tree)
		return (0);
	int	i;
	if (tree->type == REDIRECT_NODE)
	{
		i = 0;
		tree->redirect->heredoc_fds = ft_malloc(20, sizeof(int));
		tree->redirect->heredocs = ft_malloc(20, sizeof(char *));
		ft_memset(tree->redirect->heredoc_fds, -1, sizeof(int) * 20);
		ft_memset(tree->redirect->heredocs, 0, sizeof(char *) * 20);
		while (tree->redirect->herdoc)
		{
			if (!create_heredoc(tree->redirect, env, i))
			{
				free(tree->redirect->heredoc_fds);
				free(tree->redirect->heredocs);
				free(tree->redirect);
				return (0);
			}
			tree->redirect->herdoc = tree->redirect->herdoc->next;
			i++;
		}
		tree->redirect->last_fd = get_lastfd(tree->redirect->heredoc_fds);
		tree->redirect->hrc_file = get_last_file(tree->redirect->heredocs);
	}
	else if (tree->type == PIPE_NODE)
	{
		open_herdocs(tree->pipe->left, env);
		open_herdocs(tree->pipe->right, env);
	}
	return (1);
}

void	collect_herdoc(t_tree *node, t_node *list)
{
	t_herdoc *h_node;

	while (list && list->type != PIPE)
	{
		if (list->type == R_IN)
			node->redirect->in_count++;
		if (list->type == R_OUT || list->type == R_APPEND)
			node->redirect->out_count++;
		if (list->type == HEREDOC && list->next)
		{
			h_node = ft_malloc(sizeof(t_herdoc), 1);
			h_node->quoted = 0;
			if (node->redirect->in)
			{
				node->redirect->hdc = 1;
				node->redirect->in = 0;
			}
			else
				node->redirect->hdc = 1;
			h_node->herdoc = ft_strdup(list->content);
			list = list->next;
			h_node->delimeter = ft_strdup(list->content);
			if (list->contain_quoted)
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
