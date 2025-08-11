/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:45:19 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 17:20:37 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	get_close_token(char *cmd, char c, int *b_space)
{
	int	len;

	len = 1;
	while (cmd[len] != c)
	{
		len++;
	}
	if (cmd[len + 1] && !is_space(cmd[len + 1]))
		*b_space = 1;
	return (len + 1);
}

size_t	get_separetor(char *cmd)
{
	int	len;

	len = 0;
	if (cmd[len + 1] && ((cmd[len] == '<' && cmd[len + 1] == '<')
			|| (cmd[len] == '>' && cmd[len + 1] == '>')))
		return (2);
	else
		return (1);
}

size_t	get_token_len(char *cmd, int *b_space)
{
	int	len;

	len = 0;
	while (cmd[len] && !is_qoute(cmd[len]) && !is_space(cmd[len])
		&& !is_sep(cmd[len]))
		len++;
	if (cmd[len] && !is_space(cmd[len]) && !is_sep(cmd[len]))
		*b_space = 1;
	return (len);
}

t_node	*new_token(char *content, int b_space)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->content = ft_strdup1(content);
	node->b_space = b_space;
	node->next = NULL;
	node->prev = NULL;
	node->between_quoted = between_quoted(content, ft_strlen(content));
	node->contain_quoted = contain_quoted(content, ft_strlen(content));
	return (node);
}
