/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:57 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/10 00:48:49 by moraouf      ###   ########.fr       */
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

static int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (global(1));
}

static void	handle_key_value(t_env **env, char *arg, char *equal_sign)
{
	char	*key;
	char	*value;

	key = ft_substr(arg, 0, equal_sign - arg);
	value = ft_strdup(equal_sign + 1);
	if (!key)
		return ;
	set_env_var(env, key, value);
}

static void	handle_no_value(t_env **env, char *arg)
{
	t_env	*existing;
	t_env	*new;

	existing = get_env_var(*env, arg);
	if (!existing)
	{
		new = create_env_var(arg, NULL);
		ft_lstadd_node(env, new);
	}
}

int	export_command(t_env **env, char **args)
{
	int		i;
	char	*equal_sign;
	int		flag;

	flag = 0;
	if (!args[1])
		return (sorted_env(*env), print_env_export(*env), global(0));
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			handle_invalid_identifier(args[i]);
			flag = 1;
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
			handle_key_value(env, args[i], equal_sign);
		else
			handle_no_value(env, args[i]);
		i++;
	}
	return (global(flag));
}
