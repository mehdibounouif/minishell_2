/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/28 11:17:36 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/history.h>

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

		printf("PIPE \n");
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
		printf("REDIRCTION \n");
		print_tree(tree->redirect->prev);
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		//print_tree(tree->redirect->prev);
		printf("redirection type : %s ", tree->redirect->redirect);
		printf("file : %s\n", tree->redirect->file);
	}
	else if (tree->type == OR_NODE)
	{
		printf("OR \n");
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		print_tree(tree->orr->left);
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		print_tree(tree->orr->right);
	}
	else if (tree->type == AND_NODE)
	{
		printf("AND \n");
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		print_tree(tree->andd->left);
		while (f--)
			printf(" ");
		f = a;
		f += 1;
		print_tree(tree->andd->right);
	}
}

int	main(int c, char **v)
{
	t_node	*list;
	t_node	*tmp;
	char	*cmd;
	t_tree	*tree;
	(void)c;
	(void)v;
	
	list = NULL;
	tree = NULL;
	int	i = 0;
	while (i < 5)
	{
		cmd = readline("> ");
		add_history(cmd);
		tokenize(cmd, &list);
		tmp = list;
		tree = pars_command(&tmp);
		print_tree(tree);
//		ft_free(&list, &tree);
		free(cmd);
		i++;
	}
	return (0);
}
