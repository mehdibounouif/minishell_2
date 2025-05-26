/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/26 10:23:56 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init(t_tree **tree)
{
	(*tree)->command->command = NULL;
	(*tree)->command->args = NULL;
	(*tree)->pipe->left = NULL;
	(*tree)->pipe->right = NULL;
	(*tree)->redirect->redirection_type = 0;
	(*tree)->redirect->file = NULL;
	(*tree)->redirect->prev = NULL;
}

void	print_tree(t_tree *tree, int f)
{
	int	i = 0;
	while (f > 0)
	{
		printf(" ");
		f--;
	}
	if (tree->type == COMMAND_NODE)
	{
		printf("COMMAND :\n");
		printf("%s , [", tree->command->command);
		while (tree->command->args[i])
			printf("%s ", tree->command->args[i++]);
		printf("]\n");
	}
	else if (tree->type == PIPE_NODE)
	{
		printf("LEFT ");
		print_tree(tree->pipe->left, f + 1);
		printf("RIGHT ");
		print_tree(tree->pipe->right, f + 1);
	}
//	else if (tree->type == REDIRECT_NODE)
//	{
//
//	}
}

int	main(int c, char **v)
{
	t_node	*list;
	t_node	*tmp;
	t_tree	*tree;
	(void)c;
	
	list = NULL;
	tree = NULL;
	tokenize(v[1], &list);
//	init(&tree);
	tmp = list;
	tree = pars_command(&tmp);
	print_tree(tree, 1);
	//print_list(list);
	return (0);
}
