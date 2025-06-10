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
#include <readline/history.h>

int	main(int c, char **v)
{
	t_node	*list;
	t_node	*tmp;
	char	*cmd;
	char	*clear_cmd;
	t_tree	*tree;
	(void)c;
	(void)v;
	
	int	i = 0;
	while (i < 5)
	{
		list = NULL;
		tree = NULL;
		cmd = readline("> ");
		add_history(cmd);
    int j = check_quotes(cmd, 100000);
    if (j == 1)
    {
      printf("double qoutes not closed!\n");
		  free(cmd);
      break;
    }
    if (j == 2)
    {
      printf("single qoutes not closed!\n");
		  free(cmd);
      break;
    }
    printf("j == %d\n", j);
    clear_cmd = remove_quotes(cmd);
		tokenize(clear_cmd, &list);
		tmp = list;
		tree = pars_command(&tmp);
		print_tree_unicode(tree, "", 1);
		//print_tree(tree)OR;
//		ft_free(&list, &tree);
		free(cmd);
		cmd	= NULL;
		i++;
	}
	return (0);
}
