/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:54:51 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 18:55:08 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_pipe_in_start(t_node **list)
{
	t_node	*tmp;
	t_node	*current;

	tmp = *list;
	while (tmp && tmp->type == PIPE)
	{
		current = tmp;
		tmp = tmp->next;
		if (tmp)
			tmp->prev = current->prev;
		if (current->prev)
			current->prev->next = tmp;
		else
			*list = tmp;
		free(current->content);
		free(current);
	}
}

int	process_command(char *cmd, t_node **list, t_env *env)
{
	int	end;
	int	start;

	if (check_quotes(cmd, ft_strlen(cmd)))
		return (free(cmd), ft_putendl_fd("Qoutes not closed!", 2), 0);
	end = 0;
	start = 0;
	tokenize(cmd, list);
	without_quotes(list);
	expand(list, env);
	if (check_pipe(*list))
		return (free_list(list), 0);
	remove_empty(list, &end);
	if (!(*list))
		return (global(0), 0);
	join_b_space_nodes(list);
	remove_pipe_in_start(list);
	remove_pipe_in_end(list, end);
	if (!(*list))
		return (0);
	if (!check_syntax(*list))
		return (free_list(list), 0);
	return (1);
}

int	readline_and_parssing(t_mini *minishell, t_env *env)
{
	char	*cmd;
	t_node	*tmp;

	cmd = readline("minishell > ");
	if (!cmd)
	{
		ft_putendl_fd("exit", 1);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(global(-1));
	}
	if (cmd[0] == '\0')
	{
		free(cmd);
		return (0);
	}
	add_history(cmd);
	if (!process_command(cmd, &minishell->list, env))
		return (0);
	tmp = minishell->list;
	minishell->tree = pars_pipe(&minishell->list);
	free_list(&tmp);
	return (1);
}
