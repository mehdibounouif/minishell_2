/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:45:19 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/07 10:34:48 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_quoted(char *cmd, int len)
{
	return ((cmd[0] == '\'' && cmd[len] == '\'') || (cmd[0] == '\"' || cmd[len] == '\''));
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

void	token_type(t_node *node, int flag)
{
	if (!node)
		return ;
	if (ft_strcmp(node->content, "") == 0)
		node->type = EMPTY;
	else if (ft_strcmp(node->content, ">") == 0 && flag == 0)
		node->type = R_OUT;
	else if (ft_strcmp(node->content, ">>") == 0 && flag == 0)
		node->type = R_APPEND;
	else if (ft_strcmp(node->content, "<<") == 0 && flag == 0)
		node->type = HEREDOC;
	else if (ft_strcmp(node->content, "<") == 0 && flag == 0)
		node->type = R_IN;
	else if (ft_strcmp(node->content, "|") == 0 && flag == 0)
		node->type = PIPE;
	else
		node->type = WORD;
}

int	len_of_sep(char	*line, int i)
{
	int	len;

	len = 0;
	if (line[i] && line[i + 1] && line[i + 2]
		&& (!ft_strncmp(&line[i], ">>", 2)
		|| !ft_strncmp(&line[i], "<<", 2)))
		len = 2;
	else if (line[i] && (!ft_strncmp(&line[i], "|", 1)
		|| (line[i] && !ft_strncmp(&line[i], ">", 1))
		|| (line[i] && !ft_strncmp(&line[i], "<", 1))))
		len = 1;
	return (len);
}

int		calc_token_byte(char *line, int *i)
{
	int		count;
	int		j;
	char	c;

	count = 0;
	j = 0;
	c = ' ';
	while (line[*i + j] && (line[*i + j] != ' ' || c != ' '))
	{
		/*
		if (is_separator(line, *i))
			return (len_of_sep(line, *i));
			*/
		if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
			c = line[*i + j++];
		else if (c != ' ' && line[*i + j] == c)
		{
			count += 2;
			c = ' ';
			j++;
		}
		else
			j++;
		if (line[*i + j - 1] == '\\')
			count--;
	}
	return (j - count + 1);
}

t_node	*get_token(char *cmd, int *i)
{
	t_node	*node;
	int		j;
	char	c;
//	int	l;
	int	len;

	j = 0;
	c = ' ';
	if (!(node = malloc(sizeof(t_node))))
		return (NULL);
	node->quoted = 0;
	len = calc_token_byte(cmd, i);
	if (!(node->content = malloc(sizeof(char) * len)))
		return (NULL);
	while (cmd[*i] && (cmd[*i] != ' ' || c != ' '))
	{
		if (is_quoted(&cmd[*i], len))
			node->quoted = 1;
		/*
		if (is_separator(cmd, *i))
		{
			l = len_of_sep(cmd, *i) + *i;
			while (*i < l)
				node->content[j++] = cmd[(*i)++];
			break;
		}
		*/
		if (c == ' ' && (cmd[*i] == '\'' || cmd[*i] == '\"'))
			c = cmd[(*i)++];
		else if (c != ' ' && cmd[*i] == c)
		{
			c = ' ';
			(*i)++;
		}
		else if (cmd[*i] == '\\' && (*i)++)
			node->content[j++] = cmd[(*i)++];
		else
			node->content[j++] = cmd[(*i)++];
	}
	node->content[j] = '\0';
	return (node);
}

int	check_real_sep(char *line, int i)
{
	if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\;", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\|", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\>", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\<", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && line[i + 2] && ft_strncmp(&line[i], "\\>>", 3) == 0)
		return (1);
	return (0);
}
