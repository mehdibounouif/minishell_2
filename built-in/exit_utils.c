/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:30:00 by taha_laylay       #+#    #+#             */
/*   Updated: 2025/08/11 01:13:54 by taha_laylay      ###   ########.fr       */
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

int	check_overflow(long long result, char digit, int negative)
{
	if (result > LLONG_MAX / 10)
		return (1);
	if (result == LLONG_MAX / 10 && (digit - '0') > LLONG_MAX % 10)
		return (1);
	if (negative && result > (LLONG_MAX / 10) + 1)
		return (1);
	return (0);
}

long long	parse_number(char *str, int *i, int sign, int *error)
{
	long long	result;

	result = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		if (check_overflow(result, str[*i], sign == -1))
		{
			*error = 1;
			return (0);
		}
		result = (result * 10) + (str[*i] - '0');
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
