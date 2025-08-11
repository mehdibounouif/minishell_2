/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 17:22:06 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

int	quotes(char *cmd)
{
	int	len;

	len = ft_strlen(cmd);
	if (len < 2)
		return (0);
	if ((cmd[0] == '\'' && cmd[len - 1] == '\'')
		|| (cmd[0] == '\"' && cmd[len - 1] == '\"'))
		return (1);
	return (0);
}

char	*remove_quotes(char *cmd)
{
	char	*clear_cmd;
	size_t	i;

	int (j), (start);
	i = 0;
	j = 0;
	if (quotes(cmd))
	{
		clear_cmd = malloc(sizeof(char) * ft_strlen(cmd) - 1);
		i = 1;
	}
	else
		clear_cmd = malloc(sizeof(char) * ft_strlen(cmd) + 1);
	start = i;
	while (cmd[i])
	{
		if (i == (ft_strlen(cmd) - 1) && start == 1)
			break ;
		clear_cmd[j++] = cmd[i++];
	}
	clear_cmd[j] = '\0';
	return (clear_cmd);
}

void	token_type(t_node *node)
{
	if (!ft_strcmp(node->content, ">"))
		node->type = R_OUT;
	else if (!ft_strcmp(node->content, ">>"))
		node->type = R_APPEND;
	else if (!ft_strcmp(node->content, "<<"))
		node->type = HEREDOC;
	else if (!ft_strcmp(node->content, "<"))
		node->type = R_IN;
	else if (!ft_strcmp(node->content, "|"))
		node->type = PIPE;
	else
		node->type = WORD;
}

size_t	calc_len(char *cmd, int i, int *b_space)
{
	size_t	len;

	len = 0;
	if (is_qoute(cmd[i]))
		len = get_close_token(&cmd[i], cmd[i], b_space);
	else if (is_sep(cmd[i]))
		len = get_separetor(&cmd[i]);
	else
		len = get_token_len(&cmd[i], b_space);
	return (len);
}

void	tokenize(char *cmd, t_node **list)
{
	size_t	len;
	char	*content;
	t_node	*token;

	int (b_space), (i), (start);
	i = 0;
	while (cmd[i])
	{
		b_space = 0;
		while (is_space(cmd[i]))
			i++;
		if (!cmd[i])
			break ;
		len = calc_len(cmd, i, &b_space);
		start = i;
		i += len;
		content = ft_substr1(cmd, start, len);
		token = new_token(content, b_space);
		free(content);
		token_type(token);
		add_back(list, token);
	}
	free(cmd);
}
