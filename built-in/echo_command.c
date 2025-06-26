/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:42:03 by moraouf           #+#    #+#             */
/*   Updated: 2025/06/25 16:42:04 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h>

int echo_command(t_env *env, char **args)
{
    int i;
    int newline;
    
    (void)env;  // env is not used in echo command
    i = 1;      // Start from first argument (skip command name)
    newline = 1; // Default to printing newline
    
    // Check for -n option
    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;  // Skip the -n option
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

