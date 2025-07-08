/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/08 13:27:11 by mbounoui         ###   ########.fr       */
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
	return (ft_strjoin(HEREDOC_FILE, nb));
}


int	create_heredoc(char *file_name, char	*delimeter, int quoted, t_env *env)
{
	int	fd;
	char	*line;

	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
	if (fd == -1)
		return (-1);
	line = readline(">");
	while (line && ft_strcmp(delimeter, line))
	{
		if (!quoted)
		{
			line = expansion(line, env);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	return (0);
}

t_tree	*parss_herdoc(t_tree *node, t_node **list, t_env *env)
{
	t_tree	*herdoc_node;
	char	*file_name;

	herdoc_node = malloc(sizeof(t_tree));
	if (!herdoc_node)
		return (NULL);
	herdoc_node->redirect = malloc(sizeof(t_redirection));
	if (!herdoc_node->redirect)
	{
		free(herdoc_node);
		return (NULL);
	}
	*list = (*list)->next;
	file_name = generate_file_name();
	if (create_heredoc(file_name, (*list)->content, (*list)->quoted, env))
		return (NULL);
	herdoc_node->type = REDIRECT_NODE;
	herdoc_node->redirect->redirect = (*list)->prev->content;
	herdoc_node->redirect->file = file_name;
	herdoc_node->redirect->prev = node;
	return (herdoc_node);
}
