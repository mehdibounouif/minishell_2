/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/27 09:54:59 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tree(t_tree *tree)
{
	int	i = 0;
	static	int	f;
	int	a;

	a  = f;
	if (tree->type == COMMAND_NODE)
	{
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		printf("COMMAND :");
		printf("%s , [", tree->command->command);
		while (tree->command->args[i])
			printf("%s ", tree->command->args[i++]);
		printf("]\n");
	}
	else if (tree->type == PIPE_NODE)
	{
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		printf("LEFT \n");
		print_tree(tree->pipe->left);
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		printf("RIGHT \n");
		print_tree(tree->pipe->right);
	}
	else if (tree->type == REDIRECT_NODE)
	{
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		printf("REDIRCTION \n");
		//print_tree(tree->redirect->prev);
		printf("redirection type : %s ", tree->redirect->redirect);
		printf("file : %s\n", tree->redirect->file);
	}
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
	tmp = list;
	tree = pars_command(&tmp);
	print_tree(tree);
//	print_list(list);
	return (0);
}
