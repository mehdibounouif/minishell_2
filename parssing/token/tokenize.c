/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/29 10:02:11 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	tokenize(char *command, t_node **list)
{
	int	i;
  t_node  *node;

	i = 0;
  node =  NULL;
	while (command[i] != '\0')
	{
		while(is_space(command[i]))
			i++;
    if (command[i] && command[i] == '#' && !check_quotes(command, i))
    {
      if (i == 0)
        return (0);
      else 
        break;
    }
		if (command[i] && command[i] == '\\')
      i++;
		if (command[i] && (command[i] == ';' && is_real_separator(command, i)))
      node = check_end(&i, 1);
		else if (command[i] && (command[i] == ';' && !is_real_separator(command, i)))
      node = check_end(&i, 0);
		else if (command[i] && (command[i] == '|' && is_real_separator(command, i)))
      node = check_pipe(&i, 1);
		else if (command[i] && (command[i] == '|' && !is_real_separator(command, i)))
      node = check_pipe(&i, 0);
		else if ((command[i] && (command[i] == '<' || command[i] == '>' || command[i] == '2') && is_real_separator(command, i)))
      node = check_redirection(command, &i, 1);
		else if ((command[i] && (command[i] == '<' || command[i] == '>') && !is_real_separator(command, i)))
      node = check_redirection(command, &i, 0);
		else if (command[i] && ((command[i] >= 33 && command[i] <= 126) || command[i] == '\"' || command[i] == '\''))
        node = check_word(command, &i);
    else
      break;
    if (!node)
    {
      free_list(list);
      return (0);
    }
		add_back(list, node);
	}
	return (1);
}
