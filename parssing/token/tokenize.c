/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/01 22:47:50 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	tokenize(char *cmd, t_node **list)
{
	int	i;
	int	start;
	int	b_space;
	size_t	len;
	char	*content;
	t_node *token;

	i = 0;
	len = 0;
	while (cmd[i])
	{
		b_space = 0;
		while (is_space(cmd[i]))
			i++;
		if (is_qoute(cmd[i]))
			len = get_close_token(&cmd[i], cmd[i], &b_space);
		else if (is_sep(cmd[i]))
			len = get_separetor(&cmd[i]);
		else
			len = get_token_len(&cmd[i], &b_space);
		start = i;
		i += len;
		content = ft_substr(cmd, start, len);
		token = new_token(content, b_space);
		token_type(token);
		add_back(list, token);
	}
}
