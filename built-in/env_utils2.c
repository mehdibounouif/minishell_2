/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 01:29:59 by moraouf       #+#    #+#             */
/*   Updated: 2025/08/10 01:32:13 by moraouf      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_size(char *str, char *str1)
{
	size_t	len;
	size_t	len1;

	len = ft_strlen(str);
	len1 = ft_strlen(str1);
	if (len > len1)
		return (len);
	return (len1);
}

void	set_env_var(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, check_size(current->key, key)) == 0)
		{
			free(current->value);
			current->value = ft_strdup1(value);
			return ;
		}
		current = current->next;
	}
	new = create_env_var(key, value);
	ft_lstadd_node(env, new);
}

// Helper function to parse environment variable string
char	**parse_env_var(char *var)
{
	char	**split_var;

	split_var = ft_split1(var, '=');
	if (!split_var || !split_var[0])
	{
		if (split_var)
			free_str(split_var);
		return (NULL);
	}
	return (split_var);
}

// Helper function to find and update existing env variable
int	find_and_update_env(t_env *env, char *key, char *value)
{
	t_env	*current;

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
