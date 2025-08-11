/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:59 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/11 19:21:06 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_numeric(char *str)
{
	int	i;
	int	sign;

	sign = 1;
	i = handle_sign(str, 0, &sign);
	if (!str[i])
		return (0);
	return (validate_digits(str, i));
}

static long long	ft_atoll(char *str, int *error)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	*error = 0;
	i = handle_sign(str, 0, &sign);
	if (!str[i] || !ft_isdigit(str[i]))
	{
		*error = 1;
		return (0);
	}
	result = parse_number(str, &i, sign, error);
	if (*error)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ' && (str[i] < 9 || str[i] > 13))
			*error = 1;
		i++;
	}
	return (result * sign);
}

static void	exit_with_error(char *arg, t_env *env)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_env(env);
	ft_free_garbage(ft_function());
	exit(2);
}

static int	calculate_exit_status(char *arg, t_env *env)
{
	long long	status;
	int			error;

	if (!is_numeric(arg))
		exit_with_error(arg, env);
	status = ft_atoll(arg, &error);
	if (error)
		exit_with_error(arg, env);
	return (normalize_exit_status(status));
}

int	exit_command(int i, t_tree *node, t_env *env, char **args)
{
	(void)node;
	if (!i)
		ft_putstr_fd("exit\n", 1);
	if (!args[1])
	{
		ft_free_garbage(ft_function());
		free_env(env);
		exit(global(-1));
	}
	if (args[2])
	{
		if (!is_numeric(args[1]))
			exit_with_error(args[1], env);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (global(1));
	}
	exit(calculate_exit_status(args[1], env));
}
