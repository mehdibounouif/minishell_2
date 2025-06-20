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

#include "../includes/minishell.h"

/*
t_node	*check_fake_pipe(int *i)
{
	t_node *node;

	node = malloc(sizeof(t_node));
  if (!node)
      return NULL;
	node->content = ft_strdup("|");
	node->type = WORD;
	node->next = NULL;
	node->prev = NULL;
	(*i)+=2;
	return (node);
}
*/
t_node	*check_pipe(int *i, int flag)
{
	t_node *node;

	node = malloc(sizeof(t_node));
  if (!node)
      return NULL;
	node->content = ft_strdup("|");
  if (flag)
	  node->type = PIPE;
  else
	  node->type = WORD; 
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
  if (!node)
     return NULL;
  if (command[*i] == '\"' || command[*i] == '\'')
    (*i)++;
  if (!check_quotes(command, *i+1))
	  while (command[*i] && !is_space(command[*i])
			  && command[*i] != '>' && command[*i] != '<'
		  	&& command[*i] != '&' && command[*i] != '|')
		  (*i)++;
  else
  {
    while (command[*i] && check_quotes(command, *i+1) != 0)
      (*i)++;
    (*i)++;
  }
	word = malloc(sizeof(int) * (*i - start));
  if (!word)
      return NULL;
	ft_strlcpy(word, command+start, (*i - start) + 1);
	node->content = word;
	node->next = NULL;
	node->prev = NULL;
	node->type = WORD;
	return (node);
}

t_node	*check_redirection(char *command, int *i, int flag)
{
	t_node *node;

	node = malloc(sizeof(t_node));
  if (!node)
      return NULL;
	if (command[*i] == '>' && command[*i+1] == '>')
	{
		node->content = ft_strdup(">>");
    if (flag)
		  node->type = REDIRECTION_APPEND;
		*i += 2;
	}
	else if (command[*i] == '<' && command[*i+1] == '<')
	{
		node->content = ft_strdup("<<");
    if (flag)
		  node->type = HEREDOC;
		*i += 2;
	}
	else if (command[*i] == '2' && command[*i+1] == '>')
	{
		node->content = ft_strdup("2>");
    if (flag)
		  node->type = REDIRECTION_ERR;
		*i += 2;
	}
	else
	{
		if (command[*i] == '>')
		{
			node->content = ft_strdup(">");
      if (flag)
		  	node->type = REDIRECTION_OUT;
		}
		else
		{
			node->content = ft_strdup("<");
      if (flag)
			  node->type = REDIRECTION_IN;
		}
		(*i)++;
	}
  if (!flag)
    node->type = WORD;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_node	*check_end_operator(int *i)
{
	t_node *node;

	node = malloc(sizeof(t_node));
  if (!node)
      return NULL;
	node->content = ft_strdup("&&");
	node->type = AND;
	node->next = NULL;
	node->prev = NULL;
	(*i) += 2;
	return (node);
}

t_node	*check_or_operator(int *i)
{
	t_node *node;

	node = malloc(sizeof(t_node));
  if (!node)
      return NULL;
	node->content = ft_strdup("||");
	node->type = OR;
	node->next = NULL;
	node->prev = NULL;
	(*i) += 2;
	return (node);
}

void	tokenize(char *command, t_node **list)
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
        exit(1);
      else 
        break;
    }
		if (command[i] && command[i] == '\\')
      i++;
    else if (command[i] && (command[i] == '|' && command[i+1] == '|'))
      node = check_or_operator(&i);
		else if (command[i] && (command[i] == '&' && command[i+1] == '&'))
      node = check_end_operator(&i);
		else if (command[i] && (command[i] == '|' && is_real_separator(command, i)))
      node = check_pipe(&i, 1);
		else if (command[i] && (command[i] == '|' && !is_real_separator(command, i)))
      node = check_pipe(&i, 0);
		else if ((command[i] && (command[i] == '<' || command[i] == '>' || command[i] == '2') && is_real_separator(command, i)))
      node = check_redirection(command, &i, 1);
		else if ((command[i] && (command[i] == '<' || command[i] == '>' || command[i] == '2') && !is_real_separator(command, i)))
      node = check_redirection(command, &i, 0);
		else if (command[i] && (command[i] == '-' || ft_isalpha(command[i]) || command[i] == '\"' || command[i] == '\''))
        node = check_word(command, &i);
    else
      break;
    if (!node)
    {
      free_list(list);
      exit(1);
    }
		add_back(list, node);
	}
}
