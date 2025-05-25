/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/25 16:04:42 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
void	check_type_word(t_node *node, t_node *list)
{
	if (list)
		while (list->next)
			list = list->next;
	if (!list)
		node->type = COMMAND1;
	else if (list->type == PIPE)
		node->type = COMMAND2;
	else if (list->type == REDIRECTION_OUT || list->type == REDIRECTION_APPEND)
		node->type = OUTFILE;
	else if (list->type == REDIRECTION_IN)
		node->type = INFILE;
	else if (list->type == REDIRECTION_ERR)
		node->type = ERRFILE;
	else if (list->type == COMMAND1 || list->type == COMMAND2)
		node->type = ARG;
}
*/

t_node	*check_pipe(int *i)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	node->content = ft_strdup("|");
	node->type = PIPE;
	node->next = NULL;
	node->prev = NULL;
	(*i)++;
	return (node);
}

t_node	*check_word(char *command, int *i)
{
	t_node	*node;
	int	start;
	char *word;
	start = *i;

	node = malloc(sizeof(t_node));
	while (command[*i] && !is_space(command[*i])
			&& command[*i] != '>' && command[*i] != '<'
			&& command[*i] != '&' && command[*i] != '|')
		(*i)++;
	word = malloc(sizeof(int) * (*i - start));
	ft_strlcpy(word, command+start, (*i - start) + 1);
	node->content = word;
	node->next = NULL;
	node->prev = NULL;
	node->type = WORD;
//	check_type_word(node, list);
	return (node);
}

t_node	*check_redirection(char *command, int *i)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (command[*i] == '>' && command[*i+1] == '>')
	{
		node->content = ft_strdup(">>");
		node->type = REDIRECTION_APPEND;
		*i += 2;
	}
	else if (command[*i] == '<' && command[*i+1] == '<')
	{
		node->content = ft_strdup("<<");
		node->type = HEREDOC;
		*i += 2;
	}
	else if (command[*i] == '2' && command[*i+1] == '>')
	{
		node->content = ft_strdup("2>");
		node->type = REDIRECTION_ERR;
		*i += 2;
	}
	else
	{
		if (command[*i] == '>')
		{
			node->content = ft_strdup(">");
			node->type = REDIRECTION_OUT;
		}
		else
		{
			node->content = ft_strdup("<");
		}
			node->type = REDIRECTION_IN;
		(*i)++;
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_node	*check_end_operator(int *i)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	node->content = ft_strdup("&");
	node->type = RUN_BACKGROUND;
	node->next = NULL;
	node->prev = NULL;
	(*i)++;
	return (node);
}

void	tokenize(char *command, t_node **list)
{
	int	i;

	i = 0;
	while (command[i] != '\0')
	{
		while(is_space(command[i]))
			i++;
		if (command[i] == '|')
			add_back(list, check_pipe(&i));
		else if (command[i] == '<' || command[i] == '>' || command[i] == '2')
			add_back(list, check_redirection(command, &i));
		else if (command[i] == '&')
			add_back(list, check_end_operator(&i));
		else 
			add_back(list, check_word(command, &i));
	}
}
