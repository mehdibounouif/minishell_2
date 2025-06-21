#include "../includes/minishell.h"

int	check_syntax(t_node *list)
{
	while (list && list->next)
	{
		// CHECK (| |)
		if (list->type == PIPE && list->next->type == PIPE)
		{
			printf("pipe error\n");
			return (0);
		}
		list = list->next;
	}
	return (1);
}

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

int readline_and_parssing(t_mini *minishell, char **env)
{
	char	*cmd;
	(void)env;
	
	// READ COMMAND
	cmd = readline("> ");
	add_history(cmd);
	// CHECK QOUTES
	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		free(cmd);
		printf("qoutes not closed!\n");
		return (0);
	}
	// TOKENIZE
  	tokenize(cmd, &minishell->list);
	if (!minishell->list)
	{
		free(cmd);
		return (0);
	}
	// CHECK SYNTAX
	if (!check_syntax(minishell->list))
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
	// GET ENV
	get_env(minishell, env);
	//  print_env(minishell->env);
	free(cmd);
	return (1);
}

t_tree *parssing_line(char *cmd, t_mini *minishell)
{
	t_tree  *tree;

	if (check_quotes(cmd, ft_strlen(cmd)))
	{
		printf("qoutes not closed!\n");
		return (NULL);
	}
	if (check_sides(minishell->list))
	{
		printf("-minishell: syntax error near unexpected token !\n");
		ft_free(minishell);
		return (NULL);
	}
	tree = pars_command(&minishell->list);
	return (tree);
}
