/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 00:26:50 by moraouf            #+#    #+#             */
/*   Updated: 2025/08/11 00:26:51 by moraouf           ###   ########.fr       */
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

int	process_command(char *cmd, t_node **list, t_env *env)
{
	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		free(cmd);
		ft_putendl_fd("Qoutes not closed!", 2);
		return (0);
	}
	tokenize(cmd, list);
	without_quotes(list);
	expand(list, env);
	if (!(*list)->content[0] && !(*list)->next && !(*list)->between_quoted)
	{
		free_list(list);
		return (0);
	}
	join_b_space_nodes(list);
	if (!check_syntax(*list))
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

	cmd = readline("minishell>");
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
