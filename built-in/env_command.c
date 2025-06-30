/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:59 by moraouf           #+#    #+#             */
/*   Updated: 2025/06/29 18:10:33 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int env_command(t_env *env, char **args)
{
    t_env *current;

    (void)args; // env command doesn't take arguments
    
    current = env;
    while (current)
    {
        if (current->value) // Only print variables that have a value
        {
            ft_putstr_fd(current->key, 1);
            ft_putchar_fd('=', 1);
            ft_putstr_fd(current->value, 1);
            ft_putchar_fd('\n', 1);
        }
        current = current->next;
    }
    return (0);
} 