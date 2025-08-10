/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:59 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/10 01:08:23 by moraouf      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h>

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(long long result, char next_digit, int is_negative)
{
	if (result > LLONG_MAX / 10)
		return (1);
	if (result == LLONG_MAX / 10 && (next_digit - '0') > LLONG_MAX % 10)
		return (1);
	if (is_negative && result > (LLONG_MAX / 10) + 1)
		return (1);
	return (0);
}

static long long	ft_atoll(char *str, int *error)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	*error = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i] || !ft_isdigit(str[i]))
	{
		*error = 1;
		return (0);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (check_overflow(result, str[i], sign == -1))
		{
			*error = 1;
			return (0);
		}
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	while (str[i])
	{
		if (str[i] != ' ' && (str[i] < 9 || str[i] > 13))
		{
			*error = 1;
			return (0);
		}
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

static void	handle_too_many_args(char *arg, t_env *env)
{
	if (!is_numeric(arg))
		exit_with_error(arg, env);
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	global(1);
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
	if (status < 0)
		status = 256 + (status % 256);
	else
		status = status % 256;
	return ((int)status);
}

int	exit_command(t_tree *node, t_env *env, char **args)
{
	(void)node;
	ft_putstr_fd("exit\n", 1);
	if (!args[1])
	{
		ft_free_garbage(ft_function());
		free_env(env);
		exit(0);
	}
	if (args[2])
	{
		handle_too_many_args(args[1], env);
		return (1);
	}
	exit(calculate_exit_status(args[1], env));
}
