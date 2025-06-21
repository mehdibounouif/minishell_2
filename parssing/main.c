/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/30 09:42:37 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int c, char **v, char **env)
{
  t_mini  *minishell;
	(void)c;
	(void)v;
	
	int	i = 0;
  if (!(minishell = ft_calloc(1, sizeof(t_mini))))
    return 0;
	while (i < 5)
	{
    // PARSSING
    if (!readline_and_parssing(minishell, env))
      continue;
    // PRINT TREE
		print_tree_unicode(minishell->tree, "", 1);
    // FREE
		ft_free(minishell);
		//free(cmd);
		i++;
	}
	return (0);
}




