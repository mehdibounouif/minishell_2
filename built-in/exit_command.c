/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:59 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/29 11:42:48 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h>

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
	// Skip leading whitespace
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i]) // No digits after sign
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0); // Non-digit found
		i++;
	}
	return (1);
}

static int	check_overflow(long long result, char next_digit, int is_negative)
{
	if (result > LLONG_MAX / 10)
		return (1); // Overflow will occur on next multiplication
	if (result == LLONG_MAX / 10 && (next_digit - '0') > LLONG_MAX % 10)
		return (1); // Overflow on adding the last digit
	if (is_negative && result > (LLONG_MAX / 10) + 1)
		return (1); // Check for LLONG_MIN overflow
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
	if (!str[i] || !ft_isdigit(str[i])) // No digits after sign
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
	while (str[i]) // Check for trailing non-whitespace characters
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

int	exit_command(t_tree *node, t_env *env, char **args)
{
	(void)node;
	(void)env;
	long long	status;
	int			error;

	ft_putstr_fd("exit\n", 1);
	if (!args[1])
		exit(0);
	if (args[2])
	{
		if(!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		global(1);
		return (1);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	status = ft_atoll(args[1], &error);
	if (error)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (status < 0)
		status = 256 + (status % 256);
	else
		status = status % 256;
	
	exit((int)status);
}
