#include "../includes/minishell.h"

void  get_env(t_mini *minishell, char **env)
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
		add_back2(&minishell->env, env_node);
		free(key_value);
		i++;
	}
}

size_t	len_to_pipe(char *cmd)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (cmd[i])
	{
		if (cmd[i] == '|' && !check_quotes(cmd, i))
			break;
		i++;
		len++;
	}
	return (len);
}

int readline_and_parssing(t_mini *minishell, char **env)
{
	char	*cmd;
	(void)env;
	size_t	len;
	
	// READ COMMAND
	cmd = readline("minishell> ");
	// handle signal here ctrl + D::
	if(!cmd)
	{
		printf("exit\n"); // free here + !!!
	 	exit(0);
	}
	add_history(cmd);
	// CHECK QOUTES
	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		free(cmd);
		printf("Qoutes not closed!\n");
		return (0);
	}
	// GET ENV
	get_env(minishell, env);
	//  print_env(minishell->env);

	//  REPLECE VARIABLE WITH VALUE
	cmd = replace_key_with_value(cmd, env, minishell->ret);
	if (!cmd)
	{
		ft_putendl_fd("33ddsd: value too great for base (error token is \"33ddsd\")", 2);
		return (0);
	}
	//printf("Command after expanding = %s\n", cmd);
	len = len_to_pipe(cmd);
	cmd = remove_quotes2(cmd, len);

	// TOKENIZE
  	tokenize(cmd, &minishell->list);
	if (!minishell->list)
	{
		free(cmd);
		return (0);
	}
	// CHECK SYNTAX
	if (!check_syntax(minishell, minishell->list))
	{
		ft_free(minishell);
		free(cmd);
		return (0);
	}
	// DESING TREE
	minishell->tree = pars_command(&minishell->list);
	if (!minishell->tree)
	{
		free(cmd);
		return(0);
	}
	free(cmd);
	return (1);
}
