/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:01:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/09 11:40:41 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
char	*to_space(char *str, int i)
{
	char	*word;
	int	index;
	int	j;
	int	len;

	index = i;
	len = 0;
	while (!is_space(str[len]))
		len++;
	word = malloc(len+1);
	if (!word)
		return (NULL);
	j = 0;
	while (len--)
		word[j++] = str[index++];
	word[j] = '\0';
	return (word);
}
*/

int	is_empty(char *s)
{
	if (ft_strlen(s))
		return (1);
	return (0);
}

int	is_redirection(t_node *node)
{
	if (node && (node->type == R_OUT ||
			node->type == R_IN ||
			node->type == R_APPEND ||
			node->type == HEREDOC))
		return (1);
	return (0);
}

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int is_separator(char *line, int i)
{
		if ((line[i] && line[i + 1]
		&& (!ft_strncmp(&line[i], ">>", 2)
		|| !ft_strncmp(&line[i], "<<", 2)))
		|| (line[i] && (!ft_strncmp(&line[i], "|", 1)
		|| !ft_strncmp(&line[i], ">", 1)
		|| !ft_strncmp(&line[i], "<", 1))))
		return (1);
	return (0);
}

int is_real_separator(char *cmd, int i)
{
 // if (i > 0 && cmd[i - 1] == '\\' &&  is_separator(&cmd[i], i))
  //  return (0);
  if (is_separator(&cmd[i], i) && check_quotes(cmd, i) == 0)
    return (1);
  else
    return (0);
}

int count_args(t_node *list)
{
  int i;

  i = 0;
  while (list && (list->type == WORD || list->quoted))
  {
    i++;
    list = list->next;
  }
  return (i);
}

void	add_back2(t_env **list, t_env *node)
{
	t_env	*tmp;
	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else 
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back(t_node **list, t_node *node)
{
	t_node	*tmp;
	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else 
	{
		tmp = *list;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
}
