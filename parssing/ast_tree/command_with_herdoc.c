/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/10 14:19:05 by mbounoui         ###   ########.fr       */
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

int	create_heredoc(t_tree *herdoc_node, t_node *list, t_env *env)
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
	while (line && ft_strcmp(list->content, line))
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
	herdoc_node->redirect->her_file = file_name;
	printf("fd = %d\n", fd);
	return (0);
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
	herdoc_node->redirect->her_redirect = list->content;
	while(list)
	{
		if (list->type == HEREDOC && list->next)
		{
			list = list->next;
			if (create_heredoc(herdoc_node, list, env))
				return (NULL);
		}
		list = list->next;
	}
	herdoc_node->type = REDIRECT_NODE;
	herdoc_node->redirect->prev = node;
	return (herdoc_node);
}
