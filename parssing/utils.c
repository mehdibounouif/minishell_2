/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:01:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/28 10:22:20 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int is_separator(char c)
{
  if (c == '|' || c == '>' || c == '<' || c == ';')
     return (1);
  return (0);
}

int is_real_separator(char *cmd, int i)
{
  if (i > 0 && cmd[i - 1] == '\\' &&  is_separator(cmd[i]))
    return (0);
  else if (is_separator(cmd[i]) && check_quotes(cmd, i) == 0)
    return (1);
  else
    return (0);
}

int count_args(t_node *list)
{
  int i;

  i = 0;
  while (list && list->type == WORD)
  {
    i++;
    list = list->next;
  }
  return (i);
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
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
}

void	print_list(t_node *list)
{
	while (list)
	{
		printf(" %s => ", list->content);
		switch (list->type) {
		case PIPE:
				printf("Type pipe (|)\n"); break;
		case WORD:
				printf("Type WORD\n"); break;
		case REDIRECTION_OUT:
				printf("Type redirection output (>)\n"); break;
		case HEREDOC:
				printf("Type heredoc (<<)\n"); break;
		case REDIRECTION_IN:
				printf("Type redirection input (<)\n"); break;
		case REDIRECTION_ERR:
				printf("Type redirection error (2)\n"); break;
		case REDIRECTION_APPEND:
				printf("Type redirection append (>>)\n"); break;
		case AND:
				printf("Type end Operator (&)\n"); break;
		}
		printf("\n");
		list = list->next;
	}
}
