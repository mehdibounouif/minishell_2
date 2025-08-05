/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:59 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/22 09:20:22 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Helper function to create a new environment variable
t_env	*create_env_var(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
  if (!new)
    return (NULL);
	new->key = ft_strdup1(key);
	new->value = ft_strdup1(value);
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
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
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
			current->value = ft_strdup1(value);
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
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			free(current->value);
			current->value = ft_strdup1(value);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

// Helper function to print environment variables
void	print_env_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd(current->key, 1);
		ft_putchar_fd('=', 1);
		ft_putstr_fd(current->value, 1);
		ft_putchar_fd('\n', 1);
		current = current->next;
	}
}
