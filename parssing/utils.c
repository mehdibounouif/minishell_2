/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:01:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 16:48:25 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_empty(char *s)
{
	if (ft_strlen(s))
		return (1);
	return (0);
}

int	is_redirection(t_node *node)
{
	if (node && (node->type == R_OUT || node->type == R_IN
			|| node->type == R_APPEND || node->type == HEREDOC))
		return (1);
	return (0);
}

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	is_separator(char *line, int i)
{
	if ((line[i] && line[i + 1] && (!ft_strncmp(&line[i], ">>", 2)
				|| !ft_strncmp(&line[i], "<<", 2))) || (line[i]
			&& (!ft_strncmp(&line[i], "|", 1) || !ft_strncmp(&line[i], ">", 1)
				|| !ft_strncmp(&line[i], "<", 1))))
		return (1);
	return (0);
}

t_node	*create_node(const char *content, int b_space, int b_q)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->content = ft_strdup1(content);
	new->b_space = b_space;
	new->between_quoted = b_q;
	new->contain_quoted = 1;
	new->type = 2;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
