/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:41:48 by moraouf           #+#    #+#             */
/*   Updated: 2025/06/25 17:42:18 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"


static int is_valid_identifier(char *str)
{
    int i;

    if (!str || !str[0])
        return (0);
    
    // First character must be a letter or underscore
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return (0);
    
    // Rest of the characters must be letters, numbers, or underscores
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}
void unset_env_var(t_env **env, char *key)
{
    t_env *current = *env;
    t_env *prev = NULL;

    // printf("%s\n", current->key);
    // printf("%s\n", key);
    while (current)
    {
        // puts(current->key);
        if (strcmp(current->key, key) == 0)
        {
            // printf("ana\n");
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int unset_command(t_env **env,char **args)
{
    int i;

    if(!args[1])
        return(EXIT_SUCCESS);
    i = 1;
    while(args[i])
    {
        if (!is_valid_identifier(args[i])) // check 
        {
            ft_putstr_fd("minishell: unset : `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            return (EXIT_FAILURE);
        }
        // printf("im here\n");
        unset_env_var(env,args[i]);
        return 1;
    }
    return(EXIT_SUCCESS);
}