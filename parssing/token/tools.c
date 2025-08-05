/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:45:19 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 20:48:20 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	contain_quoted(char *cmd, int len)
{
	int	i;
	if (!cmd)
		return (0);
	i = 0;
	while (i < len)
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	between_quoted(char *cmd, int len)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (1);
	else if (cmd[0] == '\"' && cmd[len - 1] == '\"')
		return (2);
	else
		return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (0);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	is_qoute(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

size_t	is_sep(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

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
	int len;

	len = 0;
	if (cmd[len + 1]
		&& ((cmd[len] == '<' && cmd[len + 1] == '<')
		|| (cmd[len] == '>' && cmd[len + 1] == '>')))
		return (2);
	else
		return (1);
}

size_t	get_token_len(char *cmd, int *b_space)
{
	int	len;

	len = 0;
	while (cmd[len] && !is_qoute(cmd[len])
		&& !is_space(cmd[len]) && !is_sep(cmd[len]))
		len++;
	if (cmd[len] && !is_space(cmd[len]) && !is_sep(cmd[len]))
		*b_space = 1;
	return (len);
}

t_node	*new_token(char *content, int b_space)
{
	t_node	*node;

	node = ft_malloc(sizeof(t_node), 1);
	node->content = ft_strdup(content);
	node->b_space = b_space;
	node->next = NULL;
	node->prev = NULL;
	node->between_quoted = between_quoted(content, ft_strlen(content));
	return (node);
}
