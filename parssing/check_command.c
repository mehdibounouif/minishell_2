#include "../includes/minishell.h"

int  get_env(t_env **envp, char **env)
{
	int i;
	char  **key_value;
	int	key_len;
	int	env_len;
	t_env *env_node;

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
	t_node *tmp;

	tmp = *list;
	while (tmp)
	{
		tmp->content = remove_quotes(tmp->content);
		tmp = tmp->next;
	}
}

int readline_and_parssing(t_mini *minishell, t_env *env)
{
	char	*cmd;

	cmd = readline("minishell>");
	if(!cmd)
	{
		printf("exit\n");
		free_env(env);
		ft_free_garbage(ft_function());
		exit(global(-1));
	}
	if(cmd[0] == '\0')
	{
		free(cmd);
		ft_free_garbage(ft_function());
		return (0);
	}
	add_history(cmd);
	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		free(cmd);
		ft_free_garbage(ft_function());
		printf("Qoutes not closed!\n");
		return (0);
	}
	tokenize(cmd, &minishell->list);
	without_quotes(&minishell->list);
  expand(&minishell->list, env);
	join_b_space_nodes(&minishell->list);
	if (!check_syntax(minishell->list))
	{
    ft_free_garbage(ft_function());
		return (0);
	}
	minishell->tree = pars_pipe(&minishell->list);
	return (1);
}
