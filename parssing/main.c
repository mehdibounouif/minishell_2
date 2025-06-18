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

int	main(int c, char **v)
{
	t_node	*list;
	t_node	*tmp;
	char	*cmd;
	t_tree	*tree;
	(void)c;
	(void)v;
	
	int	i = 0;
	while (i < 5)
	{
		list = NULL;
		tree = NULL;
		tmp = list;

    // READ COMMAND
		cmd = readline("> ");
		add_history(cmd);

    // PARSSING
		tree = parssing(cmd, tmp);
    if (!tree)
    {
      printf("Parssing failed!\n");
		  free(cmd);
      break;
    }

    // PRINT TREE
		print_tree_unicode(tree, "", 1);

    // FREE
		ft_free(&list, &tree);
		free(cmd);
		i++;
	}
	return (0);
}
