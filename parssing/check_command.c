#include "../includes/minishell.h"

char	*ft_strdup2(const char *s)
{
	size_t	i;
	char	*res;
	size_t	len;

	if(!s)
		return NULL;
	len = ft_strlen(s) + 1;
	res = malloc(sizeof(char) * len);
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
int  get_env(t_env **envp, char **env)
{
	int i;
	int j;
	char  **key_value;
	int	key_len;
	int	env_len;
	t_env *env_node;

	i = 0;
	while (env[i])
	{
		j = 1;
		env_node = malloc(sizeof(t_env));
		key_value = ft_split(env[i], '=');
		env_node->key = ft_strdup2(key_value[0]);
		key_len = ft_strlen(env_node->key);
		env_len = ft_strlen(env[i]);
		env_node->value = ft_substr(env[i], key_len + 1, (env_len - key_len));
		env_node->next = NULL;
		add_back2(envp, env_node);
		free_str(key_value);
		i++;
	}
	return (1);
}

int readline_and_parssing(t_mini *minishell, t_env *env)
{
	char	*cmd;
	t_node *tmp;
//	char	*prompt;

	//prompt = ft_prompt(env);
	cmd = readline("minishell >");
	if(!cmd)
	{
		printf("exit\n");
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
	cmd = expansion(cmd, env);
	if(cmd[0] == '\0')
		return (0);
	tokenize(cmd, &minishell->list);
	if (!check_syntax(minishell->list))
	{
		free_list(&minishell->list);
		free(cmd);
		return (0);
	}
	tmp = minishell->list;
	minishell->tree = pars_command(&minishell->list);
	//free_list(&tmp);
	//free(cmd);
	return (1);
}
