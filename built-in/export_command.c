/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:57 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/27 22:46:24 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_env_export(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->key, 1);
		if (current->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		current = current->next;
	}
}

int	export_command(t_env *env, char **args)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;
	t_env	*existing;
	t_env	*new;

	i = 1;
	if (!args[1])
	{
		sorted_env(env);
		print_env_export(env);
		return (EXIT_SUCCESS);
	}
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			global(1);
			return(1);
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			key = ft_substr(args[i], 0, equal_sign - args[i]);
			value = ft_strdup(equal_sign + 1);
			if (!key || !value)
				return (free(key), free(value), EXIT_FAILURE);
			existing = get_env_var(env, key);
			if (existing)
			{
				free(existing->value);
				existing->value = ft_strdup(value);
			}
			else
			{
				set_env_var(&env, key, value);
			}
			free(key);
			//free(value);
		}
		else
		{
			existing = get_env_var(env, args[i]);
			if (!existing)
			{
				// add at the last hna makynach
				new = create_env_var(args[i], NULL);
				ft_lstadd_node(&env, new);
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
