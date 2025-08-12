/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:30:00 by taha_laylay       #+#    #+#             */
/*   Updated: 2025/08/13 00:09:51 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_sign(char *str, int i, int *sign)
{
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

int	validate_digits(char *str, int start)
{
	int	i;

	i = start;
	if (!str[i] || !ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			while (str[i])
			{
				if (str[i] != ' ' && (str[i] < 9 || str[i] > 13))
					return (0);
				i++;
			}
			break ;
		}
		i++;
	}
	return (1);
}

long long	parse_number(char *str, int *i, int sign, int *error)
{
	long long	result;

	result = 0;
	while (str[*i])
	{
		int digit = str[*i] - '0';
		if (str[*i] >= '0' && str[*i] <= '9')
		{
			if (sign > 0)
			{
				if (result > ((LLONG_MAX - digit) / 10))
					*error = 1;
			}
			else if (sign < 0)
			{
				if (-result < ((LLONG_MIN + digit) / 10))
					*error = 1;
			}
		}
		result = (result * 10) + digit;
		(*i)++;
	}
	return (result);
}

int	normalize_exit_status(long long status)
{
	if (status < 0)
		status = 256 + (status % 256);
	else
		status = status % 256;
	return ((int)status);
}
