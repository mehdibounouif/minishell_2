/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:45:19 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/15 14:57:04 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_quoted(char *cmd, int len)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while(i < len)
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			return (1);
		i++;
	}
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
		if (is_separator(line, *i))
			return (len_of_sep(line, *i));
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
	int	l;
	int	h;
	int	is_sep;
	int	len;

	j = 0;
	c = ' ';
	is_sep = 0;
	if (!(node = malloc(sizeof(t_node))))
		return (NULL);
	node->quoted = 0;
	len = calc_token_byte(cmd, i);
	if (!(node->content = malloc(sizeof(char) * len)))
		return (NULL);
	l = *i;
	while (cmd[*i] && (cmd[*i] != ' ' || c != ' '))
	{
		if (is_quoted(&cmd[l], len - 1))
		{
			node->quoted = 1;
			l = len;
		}	
		if (is_separator(cmd, *i) && !check_quotes(cmd, *i))
		{
			h = len_of_sep(cmd, *i) + *i;
			is_sep = 1;
		}
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
		if (is_sep && h == *i)
			break;
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
