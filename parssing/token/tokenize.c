/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 11:44:41 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

char  *remove_quotes(char *cmd)
{
	char  *clear_cmd;
	int	(len), (i), (j), (start);

	len = ft_strlen(cmd);
	i = 0;
	j = 0;
	if ((cmd[i] == '\'' && cmd[len - 1] == '\'')
		|| (cmd[i] == '\"' && cmd[len - 1] == '\"') )
	{
		clear_cmd = malloc(sizeof(int)* len - 1);
		i = 1;
	}
	else 
	{
		clear_cmd = malloc(sizeof(int)* len + 1);
		i = 0;
	}
	start = i;
	while (cmd[i])
	{
		if (i == (len - 1) && start == 1)
			break;
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
    if (!cmd[i])
      break;
		if (is_qoute(cmd[i]))
			len = get_close_token(&cmd[i], cmd[i], &b_space);
		else if (is_sep(cmd[i]))
			len = get_separetor(&cmd[i]);
		else
			len = get_token_len(&cmd[i], &b_space);
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
