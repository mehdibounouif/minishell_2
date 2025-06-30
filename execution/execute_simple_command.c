#include "../includes/minishell.h"

// Function to check if a command is a builtin
static int is_builtin(char *command)
{
	if (!command)
		return (0);
	
	if (ft_strncmp(command, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(command, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	
	return (0);
}

// Function to execute builtin commands
static int execute_builtin(t_tree *node, t_env *env)
{
	char *command = node->command->command;
	
	if (ft_strncmp(command, "cd", 3) == 0)
		return (cd_command(env, node->command->args));
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (echo_command(env, node->command->args));
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (pwd_command(env, node->command->args));
	else if (ft_strncmp(command, "export", 7) == 0)
		return (export_command(env, node->command->args));
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (unset_command(env, node->command->args));
	else if (ft_strncmp(command, "env", 4) == 0)
		return (env_command(env, node->command->args));
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (exit_command(env, node->command->args));
	
	return (1); // Should not reach here
}

int execute_command_node(t_tree *node, char **env)
{
	// Check if it's a builtin command first
	if (is_builtin(node->command->command))
	{
		// For builtins, we need to convert char **env to t_env *env
		// This is a temporary solution - you might want to pass t_env *env directly
		// from the main function instead of char **env
		
		// Create a temporary t_env structure from char **env
		t_env *env_list = NULL;
		int i = 0;
		while (env[i])
		{
			char *equal_sign = ft_strchr(env[i], '=');
			if (equal_sign)
			{
				char *key = ft_substr(env[i], 0, equal_sign - env[i]);
				char *value = ft_strdup(equal_sign + 1);
				t_env *new_env = create_env_var(key, value);
				ft_lstadd_node(&env_list, new_env);
				free(key);
				free(value);
			}
			i++;
		}
		
		int result = execute_builtin(node, env_list);
		
		// Free the temporary env list
		free_env(env_list);
		
		return (result);
	}
	
	// If not a builtin, execute as external command
	pid_t pid = fork();

	if (pid == 0) // child process
	{
		char *path = find_path(node, env); // ls 
		// execute command
		// replaces the child process with the new
		// if success does not return anything.
		execve(path, node->command->args, env);
		perror("execve");
		exit(1);
	}
	else
	{
		// status is variable to store exit info 
		// like
		// if child exit normally (exit() or return)
		// if child killed by a signal
		// if there is core dump
		// So the exit code is not the whole integer 
		// — it's stored in specific bits only 
		// (usually the lower 8 bits, depending on the system).
		int status;
		waitpid(pid, &status, 0);
		// WEXITSTATUS(status)
		// extract only the meaningful exit code bits from status
		return WEXITSTATUS(status);
	}
}
