#include "../includes/minishell.h"
#include "../libft/libft.h"
#include "built-in.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// Simple environment structure for testing
// typedef struct s_env
// {
//     char    *key;
//     char    *value;
//     struct s_env *next;
// } t_env;

// Helper function to create a new environment variable
t_env	*create_env_var(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

// Helper function to add environment variable
void	add_env_var(t_env **env, char *key, char *value)
{
	t_env	*new;

	new = create_env_var(key, value);
	if (!new)
		return ;
	new->next = *env;
	*env = new;
}

// Helper function to get environment variable value
char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

// set new var at the env
void	set_env_var(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	// add new variable || not found
	new = create_env_var(key, value);
	ft_lstadd_node(env, new);
}

// Helper function to update environment variable
int	update_env_var(t_env *env, char *var)
{
	char	*key;
	char	*value;
	t_env	*current;

	key = strtok(var, "=");
	value = strtok(NULL, "=");
	current = env;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

// Helper function to free environment variables
void	free_env(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

// Helper function to print environment variables
void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	printf("\nCurrent Environment Variables:\n");
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	printf("\n");
}

void	test_command(t_env *env, char *cmd_name, char **args)
{
	int	result;

	printf("\n=== Testing %s Command ===\n", cmd_name);
	if (strcmp(cmd_name, "cd") == 0)
		result = cd_command(env, args);
	else if (strcmp(cmd_name, "echo") == 0)
		result = echo_command( args);
	else if (strcmp(cmd_name, "pwd") == 0)
		result = pwd_command(env, args);
	else if (strcmp(cmd_name, "export") == 0)
		result = export_command(env, args);
	// else if (strcmp(cmd_name, "exit") == 0)
	// 	result = exit_command(tree,env, args);
	else if (strcmp(cmd_name, "unset") == 0)
	{
		// print_env(env);
		result = unset_command(env, args);
	}
	else if (strcmp(cmd_name, "env") == 0)
		result = env_command(env, args);
	else
	{
		printf("Command %s not implemented yet\n", cmd_name);
		return ;
	}
	printf("Result: %d\n", result);
	// print_env(env);
}

int	global(int state)
{
	static int	value;

	if (state != -1)
		value = state;
	return (value);
}

int	main(void)
{
	t_env	*env;
	// t_tree	*tree;
	char	*args[10];
	char	*line;
	char	*cmd;
	int		i;
			char *arg;

	env = NULL;
	// tree = NULL;
	// Initialize environment variables
	add_env_var(&env, "HOME", getenv("HOME"));
	add_env_var(&env, "PWD", getcwd(NULL, 0));
	add_env_var(&env, "OLDPWD", getcwd(NULL, 0));
	add_env_var(&env, "PATH", getenv("PATH"));
	add_env_var(&env, "USER", getenv("USER"));
	printf("Welcome to Minishell Test Environment\n");
	printf("Type 'exit' to quit\n\n");
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("\n");
			break ;
		}
		if (*line) // if line is not empty
			add_history(line);
		// if (strcmp(line, "exit") == 0)
		// {
		//     free(line);
		//     break ;
		// }
		// Parse command and arguments
		cmd = strtok(line, " ");
		if (cmd)
		{
			args[0] = cmd;
			i = 1;
			while ((arg = strtok(NULL, " ")) != NULL && i < 9)
				args[i++] = arg;
			args[i] = NULL;
			test_command(env, cmd, args);
		}
		free(line);
	}
	free_env(env);
	return (0);
}
