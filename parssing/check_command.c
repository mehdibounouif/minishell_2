/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>              +#+  +:+       +#+      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 00:26:50 by moraouf            #+#    #+#            */
/*   Updated: 2025/08/11 23:46:29 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_env(t_env **envp, char **env)
{
	int		i;
	char	**key_value;
	int		key_len;
	int		env_len;
	t_env	*env_node;

	i = 0;
	while (env[i])
	{
		env_node = malloc(sizeof(t_env));
		key_value = ft_split1(env[i], '=');
		env_node->key = ft_strdup1(key_value[0]);
		key_len = ft_strlen(env_node->key);
		env_len = ft_strlen(env[i]);
		env_node->value = ft_substr1(env[i], key_len + 1, (env_len - key_len));
		env_node->next = NULL;
		add_back2(envp, env_node);
		free_str(key_value);
		i++;
	}
	return (1);
}

void	without_quotes(t_node **list)
{
	t_node	*tmp;
	char	*content;

	tmp = *list;
	while (tmp)
	{
		content = tmp->content;
		tmp->content = remove_quotes(tmp->content);
		free(content);
		tmp = tmp->next;
	}
}

void remove_empty(t_node **list, int *start, int *end)
{
	t_node *tmp;
	t_node *next;
	t_node *prev;
	int	flag;

	tmp = *list;
	flag = 0;
	while (tmp)
	{
		if (tmp->content && tmp->content[0] == '\0' && !tmp->between_quoted)
		{
			*start = 1;
			if (flag)
				*end = 1;
			next = tmp->next;
			prev = tmp->prev;
			if (prev)
				prev->next = next;
			else
				*list = next;
			if (next)
				next->prev = prev;
			free(tmp->content);
			free(tmp);
			tmp = next;
		}
		else
		{
			if (tmp->type != PIPE)
				flag = 1;
			tmp = tmp->next;
		}
	}
}

void remove_pipe_in_end(t_node **list, int flag)
{
	t_node *tmp;
	t_node *current;

	if (!list || !*list)
		return;
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	while (tmp && tmp->type == PIPE)
	{
		if (flag)
			if (tmp->type == PIPE && tmp->prev && tmp->prev->type != PIPE)
				break;
		current = tmp;
		tmp = tmp->prev;
		if (tmp)
			tmp->next = current->next;
		else
			*list = NULL;
		free(current->content);
		free(current);
	}
}

void remove_pipe_in_start(t_node **list, int flag)
{
	t_node *tmp;
	t_node *current;

	tmp = *list;
	while (tmp && tmp->type == PIPE)
	{
		if (!flag)
			if (tmp->type == PIPE && tmp->next->type != PIPE)
				break;
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
/*
void	print_list(t_node *list)
{
	while (list)
	{
		printf(" %s ", list->content);
		list = list->next;
	}
	printf("\n");
}
*/
int	process_command(char *cmd, t_node **list, t_env *env)
{
	int end;
	int start;
	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		free(cmd);
		ft_putendl_fd("Qoutes not closed!", 2);
		return (0);
	}
	end = 0;
	start = 0;
	tokenize(cmd, list);
	without_quotes(list);
	expand(list, env);
	if (!(*list))
		return (0);
	remove_empty(list, &start,  &end);
	if (!(*list))
		return (0);
	join_b_space_nodes(list);
	remove_pipe_in_start(list, start);
	remove_pipe_in_end(list, end);
	if (!(*list))
		return (0);
	if (!check_syntax(*list, end))
	{
		free_list(list);
		return (0);
	}
	return (1);
}

int	readline_and_parssing(t_mini *minishell, t_env *env)
{
	char	*cmd;
	t_node	*tmp;

	cmd = readline("minishell > ");
	if (!cmd)
	{
		printf("exit\n");
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
