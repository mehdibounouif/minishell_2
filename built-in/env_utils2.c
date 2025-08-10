/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 01:29:59 by taha_laylay       #+#    #+#             */
/*   Updated: 2025/08/10 01:32:13 by taha_laylay      ###   ########.fr       */
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
