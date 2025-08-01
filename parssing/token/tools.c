/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:45:19 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/01 22:32:17 by mbounoui         ###   ########.fr       */
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
	if (cmd[0] == '\'' && cmd[len] == '\'')
		return (1);
	else if (cmd[0] == '\"' && cmd[len] == '\"')
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

size_t	get_close_token(char *cmd, char c)
{
	int	len;

	len = 1;
	while (cmd[len] != c)
	{
		len++;
	}
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

size_t	get_token_len(char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] && !is_qoute(cmd[len]) && !is_space(cmd[len]) && !is_sep(cmd[len]))
	{
		len++;
	}
	return (len);
}

t_node	*new_token(char *content)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->content = ft_strdup(content);
	node->next = NULL;
	node->prev = NULL;
	node->between_quoted = between_quoted(content, ft_strlen(content));
	return (node);
}
