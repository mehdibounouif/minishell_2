/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:42:03 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/06 11:54:14 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int echo_command(t_env *env, char **args)
{
    int i;
    int j;
	int	len;
    int newline;
    
    (void)env;  // env is not used in echo command
    i = 1;      // Start from first argument (skip command name)
    j = 1;      // Start from first argument (skip command name)
    newline = 1; // Default to printing newline
    
    // Check for -n option
    if (args[1] && !ft_strncmp(args[1], "-n", 2))
    {
		// I HANDEL THIS CASE echo -nnnnnnnnnnnnnnn hello
		len = ft_strlen(args[1]);
		while (args[1][j])
		{
		if (args[1][j] != 'n')
				break;
			j++;
		}
		if (j == len)
		{
			newline = 0;
			i = 2;  // Skip the -n option
		}
    }
    
    // Print all arguments with spaces between them
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', 1);
    
    return (0);
}

