#include "../includes/minishell.h"

void  get_env(t_env **envp, char **env)
{
	int i;
	char  **key_value;
	t_env *env_node;

	i = 0;
	while (env[i])
	{ 
		if (!(env_node = malloc(sizeof(t_env)))) 
		{ 
			printf("env malloc failed!\n"); 
			return ;
		}
		key_value = ft_split(env[i], '=');
		env_node->key = ft_strdup(key_value[0]);
		env_node->value = ft_strdup(key_value[1]);
		env_node->next = NULL;
		add_back2(envp, env_node);
		free_str(key_value);
		i++;
	}
}

int readline_and_parssing(t_mini *minishell, t_env *env)
{
	char	*cmd;

	cmd = readline("minishell> ");
	if(!cmd)
	{
		free_env(env);
	 	exit(2);
	}
	add_history(cmd);
	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		free(cmd);
		printf("Qoutes not closed!\n");
		return (0);
	}
	cmd = expansion(cmd, env);
	if (!cmd)
	{
		ft_putendl_fd("33ddsd: value too great for base (error token is \"33ddsd\")", 2);
		return (0);
	}
  	tokenize(cmd, &minishell->list);
	if (!minishell->list)
	{
		free(cmd);
		return (0);
	}
	if (!check_syntax(minishell->list))
	{
		free_list(&minishell->list);
		free(cmd);
		return (0);
	}
	t_node *tmp = minishell->list;
	minishell->tree = pars_command(&minishell->list);
	if (!minishell->tree)
	{
		free(cmd);
		return(0);
	}
	free_list(&tmp);
	free(cmd);
	return (1);
}

