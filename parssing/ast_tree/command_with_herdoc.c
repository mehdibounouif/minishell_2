/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 13:53:15 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/07/24 13:25:42 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct	s_share3
{
	int	fd;
	char	*line;
	char	*file_name;
	char	*expand_line;
}	t_share3;

static char	*generate_file_name(void)
{
	unsigned char	c;
	char			*buffer;

	int fd, i;
	buffer = ft_malloc(sizeof(char), 9);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (buffer);
	i = 0;
	while (i < 8)
	{
		if (read(fd, &c, 1) != 1)
		{
			close(fd);
			return (NULL);
		}
		buffer[i++] = 'a' + (c % 26);
	}
	buffer[i] = '\0';
	close(fd);
	return (buffer);
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

int	check_line(t_share3 *share)
{
	if(!share->line)
	{
		free(share->file_name);
		close(share->fd);
		return (0);
	}
	return (1);
}

int	read_lines(t_share3 *share, t_redirection *list, t_env *env)
{
	while (share->line && ft_strcmp(list->herdoc->delimeter, share->line))
	{
		if (!list->herdoc->quoted)
		{
			share->expand_line = expansion(share->line, env);
			write(share->fd, share->expand_line, ft_strlen(share->expand_line));
			write(share->fd, "\n", 1);
		}
		else 
		{
			write(share->fd, share->line, ft_strlen(share->line));
			write(share->fd, "\n", 1);
			free(share->line);
		}
		share->line = readline(">");
		if (!check_line(share))
		return (0);
	}
	return (1);
}

int	create_heredoc(t_redirection *list, t_env *env, int i)
{
	t_share3 *share;

	share = ft_malloc(sizeof(t_share3), 1);
	share->file_name = generate_file_name();
	share->fd = open(share->file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
	if (share->fd == -1)
	{
		free(share->file_name);
		printf("open heredoc file failed!\n");
		return (0);
	}
	list->heredocs[i] = ft_strdup(share->file_name);
	free(share->file_name);
	share->line = readline(">");
	if (!check_line(share))
		return (0);
	if (!read_lines(share, list, env))
		return (0);
	close(share->fd);
	return (1);
}

int	open_her(t_tree *tree, t_env *env)
{
	int	i;

	i = 0;
	tree->redirect->heredocs = ft_malloc(20, sizeof(char *));
	ft_memset(tree->redirect->heredocs, 0, sizeof(char *) * 20);
	while (tree->redirect->herdoc)
	{
		if (!create_heredoc(tree->redirect, env, i))
		{
			free(tree->redirect->heredocs);
			free(tree->redirect);
			return (0);
		}
		tree->redirect->herdoc = tree->redirect->herdoc->next;
		i++;
	}
	tree->redirect->hrc_file = get_last_file(tree->redirect->heredocs);
	return (0);
}

int	open_herdocs(t_tree *tree, t_env *env)
{
	if (!tree)
		return (0);
	if (tree->type == REDIRECT_NODE)
	{
		if (open_her(tree, env))
			return (0);
	}
	else if (tree->type == PIPE_NODE)
	{
		open_herdocs(tree->pipe->left, env);
		open_herdocs(tree->pipe->right, env);
	}
	return (1);
}

void	take_heredoc(t_tree *node, t_node *list)
{
	t_herdoc *h_node;

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

void	collect_herdoc(t_tree *node, t_node *list)
{
	while (list && list->type != PIPE)
	{
		if (list->type == R_IN)
			node->redirect->in_count++;
		else if (list->type == R_OUT || list->type == R_APPEND)
			node->redirect->out_count++;
		if (list->type == HEREDOC && list->next)
			take_heredoc(node, list);
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
